#include "PQConnection.h"

#include "PostgreSQLModule.h"
#include "Async/Async.h"

/** WINDOWS */
#if PLATFORM_WINDOWS
// Winsock Includes
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define SECURITY_WIN32

#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Secur32.lib")
#endif
/** WINDOWS */

/** LINUX */
#if PLATFORM_LINUX
#endif
/** LINUX */

#include <time.h>

#include <pqxx/pqxx>

FPQConnection::FPQConnection(const FPQConnectionString& ConnectionString)
	: ConnectionString(ConnectionString.ToString()) { }

FPQConnection::FPQConnection(const FString& ConnectionString)
	: ConnectionString(ConnectionString) { }

FPQConnection::FPQConnection(const FPQConnection& Other)
	: ConnectionString(Other.ConnectionString),
	Connection(Other.Connection) { }

FPQConnection::~FPQConnection()
{
	FPQConnection::Disconnect();
}

bool FPQConnection::Connect()
{
	if (IsConnected())
		return true;
	
	Connection.Reset();
	try
	{
		Connection = MakeShared<pqxx::connection, ESPMode::ThreadSafe>(TCHAR_TO_ANSI(*ConnectionString));
		if (Connection->is_open())
		{
			UE_LOG(LogPQ, Log, TEXT("Successfully connected to the database"));
		}
		else
		{
			UE_LOG(LogPQ, Error, TEXT("Error connecting to the database"));
			return false;
		}
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogPQ, Error, TEXT("Error connecting to the database, error: %s"), ANSI_TO_TCHAR(e.what()));
		return false;
	}

	return true;
}

bool FPQConnection::IsValid(FString& OutMessage) const
{
	// @todo
	return true;
}

bool FPQConnection::Disconnect()
{
	if (Connection.IsValid() && IsConnected())
	{
		Connection->disconnect();
		Connection.Reset();
		return true;
	}

	return false;
}

bool FPQConnection::IsConnected() const
{
	if (Connection.IsValid())
		return Connection->is_open();
	
	return false;
}

bool FPQConnection::Execute(const FString& Query, const FDbPlusTransaction& Transaction) const
{
	check(IsConnected());

	try
	{
		//pqxx::work Transaction = TransactionName.IsEmpty() ? pqxx::work(*Connection) : pqxx::work(*Connection, TCHAR_TO_ANSI(*TransactionName));
		pqxx::work PQTransaction(*Connection);
		auto PQResult = PQTransaction.exec(TCHAR_TO_ANSI(*Query));
		PQTransaction.commit();
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogPQ, Error, TEXT("Error executing query! Error: %s"), ANSI_TO_TCHAR(e.what()));
		return false;
	}

	// #todo Return status from Result
	return true;
}

bool FPQConnection::Query(const FString& Query, FDbPlusRecordSet& OutResult, const FDbPlusTransaction& Transaction) const
{
	check(IsConnected());

	pqxx::result Result;
	try
	{
		//pqxx::work Transaction = TransactionName.IsEmpty() ? pqxx::work(*Connection) : pqxx::work(*Connection, TCHAR_TO_ANSI(*TransactionName));
		pqxx::work PQTransaction(*Connection);
        Result = PQTransaction.exec(TCHAR_TO_ANSI(*Query));
		PQTransaction.commit();

		for (auto i = 0; i < StaticCast<int32>(Result.size()); i++)
		{
            auto PQRow = Result.at(i);
			FDbPlusRow Row;
			DecodeRow(&PQRow, Row);
			OutResult.AppendRow(MoveTemp(Row));
		}
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogPQ, Error, TEXT("Error executing query! Error: %s"), ANSI_TO_TCHAR(e.what()));
		return false;
	}

	return Result.size() > 0;
}

void FPQConnection::DecodeRow(pqxx::row* InRow, FDbPlusRow& OutRow) const
{
	for (auto i = 0; i < StaticCast<int32>(InRow->size()); i++)
	{
		auto RowField = (*InRow)[i];

		auto ColumnName = FString(ANSI_TO_TCHAR((char*)(*InRow)[i].name()));
		FDbPlusField Field;
		const auto bIsNull = RowField.is_null();

		const auto PQType = (*InRow)[i].type();
		switch (PQType)
		{
		case 17: // Byte
			Field = bIsNull
				? FDbPlusField(EDbPlusDataType::DPDT_UInt8)
				: FDbPlusField((uint8)RowField.as<int32>());
			break;

		case 21: // Int (2 bit)
			Field = bIsNull
				? FDbPlusField(EDbPlusDataType::DPDT_Int16)
				: FDbPlusField(RowField.as<int16>());
			break;

		case 23: // Int (4 bit)
			Field = bIsNull
				? FDbPlusField(EDbPlusDataType::DPDT_Int32)
				: FDbPlusField(RowField.as<int32>());
			break;

		case 20: // Int (8 bit)
			Field = bIsNull
				? FDbPlusField(EDbPlusDataType::DPDT_Int64)
				: FDbPlusField(RowField.as<int64>());
			break;

		case 700:
			Field = bIsNull
				? FDbPlusField(EDbPlusDataType::DPDT_Float)
				: FDbPlusField(RowField.as<float>());
			break;

		case 701:
			Field = bIsNull
				? FDbPlusField(EDbPlusDataType::DPDT_Double)
				: FDbPlusField(RowField.as<double>());
			break;

		case 1263: // C_STR
		case 2275:
		case 1043: // VARCHAR
		case 25: // Text
			Field = bIsNull
				? FDbPlusField(EDbPlusDataType::DPDT_String)
				: FDbPlusField(FString(RowField.as<std::string>().c_str()));
			break;

		//case 18: // Char
		//	Field = bIsNull
		//		? FPQField(EDbPlusDataType::DPDT_Char)
		//		: FPQField(StaticCast<TCHAR*>(ANSI_TO_TCHAR(RowField.as<std::string>().c_str())[0]));
		//	break;

		case 16: // Boolean
			Field = bIsNull
				? FDbPlusField(EDbPlusDataType::DPDT_Boolean)
				: FDbPlusField(RowField.as<bool>());
			break;

		default:
			ensureAlwaysMsgf(false, TEXT("Column type %i was unhandled."), (*InRow)[i].type());
			checkNoEntry();
		}

		OutRow.Fields.Add(ColumnName, Field);
	}
}
