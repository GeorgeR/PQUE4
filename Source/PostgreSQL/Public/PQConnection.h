#pragma once

#include "CoreMinimal.h"
#include "Future.h"

#include "PQConnectionString.h"
#include "PQData.h"

#include <pqxx/pqxx>

struct POSTGRESQL_API FPQQueryResult
{
public:
	bool bWasSuccessful;
	TArray<FPQRow> Rows;
};

class POSTGRESQL_API FPQConnection
{
public:
	FPQConnection(const FPQConnectionString& ConnectionString);
	FPQConnection(const FString& ConnectionString);

	FPQConnection(const FPQConnection& Other);

	virtual ~FPQConnection();

	bool Connect();
	TFuture<bool> ConnectAsync();

	void Disconnect();

	bool IsOpen() const;
	//TSharedRef<FPQWork> BeginTransaction();

	bool Execute(const FString& SQL, const FString& TransactionName = TEXT("")) const;
	TFuture<bool> ExecuteAsync(const FString& SQL, const FString& TransactionName = TEXT(""));

	bool Query(const FString& SQL, TArray<FPQRow>& Rows, const FString& TransactionName = TEXT("")) const;
	TFuture<FPQQueryResult> QueryAsync(const FString& SQL, const FString& TransactionName = TEXT(""));

private:
	FString ConnectionString;
	TSharedPtr<pqxx::connection, ESPMode::ThreadSafe> Connection;
};
