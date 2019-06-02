#include "PQConnection.h"

#include "PostgreSQLModule.h"
#include "Async.h"

/** WINDOWS */
#if PLATFORM_WINDOWS
// Winsock Includes
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define SECURITY_WIN32

#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <sspi.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include "Windows/HideWindowsPlatformTypes.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Secur32.lib")
#endif
/** WINDOWS */

/** LINUX */
#if PLATFORM_LINUX
#endif
/** LINUX */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>

#include <iostream>
#include <pqxx/pqxx>

//FDbRow::FDbRow(pqxx::row* Row)
//{
//	for (auto i = 0; i < Row->size(); i++)
//	{
//		TPair<FString, TUnion> Pair;
//		Pair.Key = ANSI_TO_TCHAR((char*)(*Row)[i].name());
//	}
//}

FPQConnection::FPQConnection(const FPQConnectionString& ConnectionString)
	: ConnectionString(ConnectionString.ToString()) { }

FPQConnection::FPQConnection(const FString& ConnectionString)
	: ConnectionString(ConnectionString) { }

FPQConnection::FPQConnection(const FPQConnection& Other)
	: ConnectionString(Other.ConnectionString),
	Connection(Other.Connection)
{
}

FPQConnection::~FPQConnection()
{
	Disconnect();
}

bool FPQConnection::Connect()
{
	if (IsOpen())
		return true;
	
	Connection.Reset();
	try
	{
		Connection = MakeShared<pqxx::connection>(TCHAR_TO_ANSI(*ConnectionString));
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

TFuture<bool> FPQConnection::ConnectAsync()
{
	return Async<bool>(EAsyncExecution::TaskGraph, [&] {
		ConnectionMutex.Lock();
		bool bResult = Connect();
		ConnectionMutex.Unlock();
		return bResult;
	});
}

void FPQConnection::Disconnect()
{
	if (Connection.IsValid() && IsOpen())
	{
		Connection->disconnect();
		Connection.Reset();
	}
}

bool FPQConnection::IsOpen() const
{
	if (Connection.IsValid())
		return Connection->is_open();
	
	return false;
}

bool FPQConnection::Execute(const FString& SQL, const FString& TransactionName)
{
	check(IsOpen());

	//pqxx::work Transaction = TransactionName.IsEmpty() ? pqxx::work(*Connection) : pqxx::work(*Connection, TCHAR_TO_ANSI(*TransactionName));
	try
	{
		pqxx::work Transaction(*Connection);
		auto Result = Transaction.exec(TCHAR_TO_ANSI(*SQL));
		Transaction.commit();
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogPQ, Error, TEXT("Error executing query! Error: %s"), ANSI_TO_TCHAR(e.what()));
		return false;
	}

	// #todo Return status from Result
	return true;
}

TFuture<bool> FPQConnection::ExecuteAsync(const FString& SQL, const FString& TransactionName /*= TEXT("")*/)
{
	return Async<bool>(EAsyncExecution::TaskGraph, [&] {
		ConnectionMutex.Lock();
		auto bWasSuccessful = Execute(SQL, TransactionName);
		ConnectionMutex.Unlock();
		return bWasSuccessful;
	});
}

bool FPQConnection::Query(const FString& SQL, TArray<FPQRow>& Rows, const FString& TransactionName)
{
	check(IsOpen());

	//pqxx::work Transaction = TransactionName.IsEmpty() ? pqxx::work(*Connection) : pqxx::work(*Connection, TCHAR_TO_ANSI(*TransactionName));
	try
	{
		pqxx::work Transaction(*Connection);
		auto Result = Transaction.exec(TCHAR_TO_ANSI(*SQL));
		Transaction.commit();

		Rows.Empty(Result.size());
		for (auto i = 0; i < (int32)Result.size(); i++)
		{
			pqxx::row NonConstRow = Result.at(i);
			FPQRow Row(&NonConstRow);
			Rows.Add(Row);
		}
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogPQ, Error, TEXT("Error executing query! Error: %s"), ANSI_TO_TCHAR(e.what()));
		return false;
	}

	// #todo Return status from Result
	return true;
}

TFuture<FPQQueryResult> FPQConnection::QueryAsync(const FString& SQL, const FString& TransactionName /*= TEXT("")*/)
{
	return Async<FPQQueryResult>(EAsyncExecution::TaskGraph, [&] {
		TArray<FPQRow> Rows;
		ConnectionMutex.Lock();
		auto bWasSuccessfull = Query(SQL, Rows, TransactionName);
		ConnectionMutex.Unlock();
		return FPQQueryResult{ bWasSuccessfull, Rows };
	});
}
