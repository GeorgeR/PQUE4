#pragma once

#include "CoreMinimal.h"
#include "Async/Future.h"

#include "PQConnectionString.generated.h"

USTRUCT(BlueprintType)
struct POSTGRESQL_API FPQConnectionString
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString DatabaseName;

	UPROPERTY(BlueprintReadOnly)
	FString User;

	UPROPERTY(BlueprintReadOnly)
	FString Password;

	UPROPERTY(BlueprintReadOnly)
	FString Address;

	UPROPERTY(BlueprintReadOnly)
	int32 Port;

	FPQConnectionString() = default;

	explicit FPQConnectionString(
		const FString& DatabaseName,
		const FString& User,
		const FString& Password,
		const FString& Address,
		int32 Port = 5432);

	FString ToString() const;
};
