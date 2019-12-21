#include "PQConnectionBP.h"

#include "PQConnection.h"

UPQConnection::UPQConnection() { }

UPQConnection::~UPQConnection() { }

void UPQConnection::SetConnection(FPQConnection* InConnection)
{
	this->Connection = MakeShareable<FPQConnection>(MoveTemp(InConnection));
}

FPQConnectionString UPQConnectionLibrary::MakeConnectionString(const FString& DatabaseName, const FString& User, const FString& Password, const FString& Address, const int32 Port /*= 5432*/)
{
	return FPQConnectionString(DatabaseName, User, Password, Address, Port);
}

UPQConnection* UPQConnectionLibrary::Connect(const FPQConnectionString& ConnectionString, bool& bWasSuccessful)
{
	auto Connection = new FPQConnection(ConnectionString);
	bWasSuccessful = Connection->Connect();

	if (bWasSuccessful)
	{
		auto Result = NewObject<UPQConnection>();
		Result->SetConnection(Connection);
		return Result;
	}

	return nullptr;
}

bool UPQConnectionLibrary::Execute(UPQConnection* Connection, const FString& Query)
{
	if (Connection == nullptr || !Connection->IsValid())
		return false;

	return Connection->Connection->Execute(Query);
}

bool UPQConnectionLibrary::Query(UPQConnection* Connection, const FString& Query, FDbPlusRecordSet& Rows)
{
	if (Connection == nullptr || !Connection->IsValid())
		return false;

	return Connection->Connection->Query(Query, Rows);
}

bool UPQConnectionLibrary::Insert(UPQConnection* Connection, const FString& TableName, const TArray<UObject*> Values, const FString& Columns, TArray<FPQRow>& Rows)
{
	if (Connection == nullptr || !Connection->IsValid())
		return false;

	FString SQL = FString::Printf(TEXT("INSERT INTO %s\n"), *TableName);
	SQL += TEXT("VALUES\n");

	for (auto& Value : Values)
	{
		//FString Row
	}
	
	return true;
}
