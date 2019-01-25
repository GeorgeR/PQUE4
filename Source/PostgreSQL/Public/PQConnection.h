#pragma once

#include "CoreMinimal.h"
#include "Future.h"

#include <pqxx/pqxx>

struct POSTGRESQL_API FPQConnectionString
{
public:
	FString DatabaseName;
	FString User;
	FString Password;
	FString Address;
	int32 Port;

	FPQConnectionString(const FString& DatabaseName,
		const FString& User,
		const FString& Password,
		const FString& Address,
		int32 Port = 5432)
		: DatabaseName(DatabaseName),
		User(User),
		Password(Password),
		Address(Address),
		Port(Port) { }

	FString ToString() const
	{
		return FString::Printf(TEXT("dbname=%s user=%s password=%s host=%s port=%i"), *DatabaseName, *User, *Password, *Address, Port);
	}
};

class POSTGRESQL_API FPQConnection
{
public:
	FPQConnection(const FPQConnectionString& ConnectionString);
	FPQConnection(const FString& ConnectionString);

	virtual ~FPQConnection();

	TFuture<bool> Connect();
	void Disconnect();
	//TSharedRef<FPQWork> BeginTransaction();

	void Execute(const FString& SQL, const FString& TransactionName = TEXT(""));
	void Query(const FString& SQL, const FString& TransactionName = TEXT(""));
	//TFuture<>

private:
	FString ConnectionString;
	TSharedPtr<pqxx::connection> Connection;	
};
