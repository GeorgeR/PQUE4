#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "PQConnectionString.h"
#include "PQData.h"

#include "PQConnectionBP.generated.h"

class FPQConnection;

/** Wrapper for FPQConnection, there is no need to call this from C++ */
UCLASS(BlueprintType)
class POSTGRESQL_API UPQConnection
	: public UObject
{
	GENERATED_BODY()

public:
	UPQConnection();
	virtual ~UPQConnection();
	
	void SetConnection(FPQConnection* Connection);

	FORCEINLINE bool IsValid() const { return ::IsValid(this) && Connection.IsValid(); }

private:
	friend class UPQConnectionLibrary;

	TSharedPtr<FPQConnection> Connection;
};

UCLASS(BlueprintType)
class POSTGRESQL_API UPQConnectionLibrary
	: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PostgreSQL")
	static FPQConnectionString MakeConnectionString(const FString& DatabaseName, const FString& User, const FString& Password, const FString& Address, const int32 Port = 5432);

	UFUNCTION(BlueprintCallable, Category = "PostgreSQL")
	static UPQConnection* Connect(const FPQConnectionString& ConnectionString, bool& bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "PostgreSQL")
	static UPQConnection* ConnectWithURL(const FString& ConnectionString, bool& bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "PostgreSQL")
	static bool Execute(UPQConnection* Connection, const FString& SQL);

	UFUNCTION(BlueprintCallable, Category = "PostgreSQL")
	static bool Query(UPQConnection* Connection, const FString& SQL, TArray<FPQRow>& Rows);

public:
	UFUNCTION(BlueprintCallable, Category = "PostgreSQL")
	static bool Insert(UPQConnection* Connection, const FString& TableName, const TArray<UObject*> Values, const FString& Columns, TArray<FPQRow>& Rows);
};
