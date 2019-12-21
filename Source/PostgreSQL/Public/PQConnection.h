#pragma once

#include "CoreMinimal.h"
#include "Future.h"

#include "Interfaces/DbPlusConnectionInterface.h"

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
	: public IDbPlusConnectionInterface
{
public:
	FPQConnection(const FPQConnectionString& ConnectionString);
	FPQConnection(const FString& ConnectionString);

	FPQConnection(const FPQConnection& Other);

	virtual ~FPQConnection();

	bool Connect() override;
	bool Disconnect() override;

	bool IsConnected() const override;
	
	bool IsValid(FString& OutMessage) const override;

	bool Execute(const FString& Query, const FDbPlusTransaction& Transaction = FDbPlusTransaction()) const override;
	bool Query(const FString& Query, FDbPlusRecordSet& OutResult, const FDbPlusTransaction& Transaction = FDbPlusTransaction()) const override;

protected:
	void DecodeRow(pqxx::row* InRow, FDbPlusRow& OutRow) const;
	
private:
	FString ConnectionString;
	TSharedPtr<pqxx::connection, ESPMode::ThreadSafe> Connection;
};
