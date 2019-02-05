#pragma once

#include "CoreMinimal.h"
#include "Union.h"

#include <pqxx/pqxx>

#include "PQData.generated.h"

UENUM(BlueprintType)
enum class EPQDataType : uint8
{
	PQ_UInt8			UMETA(DisplayName = "Byte"),
	PQ_Int16			UMETA(DisplayName = "Integer16"),
	PQ_Int32			UMETA(DisplayName = "Integer32"),
	PQ_Int64			UMETA(DisplayName = "Integer64"),

	PQ_Float			UMETA(DisplayName = "Float"),
	PQ_Double			UMETA(DisplayName = "Double"),

	PQ_String			UMETA(DisplayName = "String"),
	PQ_Char				UMETA(DisplayName = "Char"),

	PQ_Boolean			UMETA(DisplayName = "Boolean"),

	PQ_Unknown = 255	UMETA(DisplayName = "Unknown (unsupported)")
};

USTRUCT(BlueprintType)
struct POSTGRESQL_API FPQField
{
	GENERATED_BODY()

public:
	FPQField() : DataType(EPQDataType::PQ_Boolean) { }

	explicit FPQField(const uint8& Value);
	explicit FPQField(const int16& Value);
	explicit FPQField(const int32& Value);
	explicit FPQField(const int64& Value);

	explicit FPQField(const float& Value);
	explicit FPQField(const double& Value);

	explicit FPQField(const FString& Value);
	explicit FPQField(const FChar& Value);

	explicit FPQField(const bool& Value);

	FORCEINLINE EPQDataType GetDataType() const { return DataType; }

	template <typename T>
	T Get() const;

	template <> uint8 Get<uint8>() const { return ByteValue; }
	template <> int16 Get<int16>() const { return Integer16Value; }
	template <> int32 Get<int32>() const { return Integer32Value; }
	template <> int64 Get<int64>() const { return Integer64Value; }

	template <> float Get<float>() const { return FloatValue; }
	template <> double Get<double>() const { return DoubleValue; }

	template <> FString Get<FString>() const { return StringValue; }
	template <> FChar Get<FChar>() const { return CharValue; }

	template <> bool Get<bool>() const { return BooleanValue; }

	FString ToString() const;

private:
	EPQDataType DataType;

	//TUnion<uint8, int16, int32, int64, float, double> ValuesA;
	//TUnion<FString, FChar, bool> ValuesB;

	uint8 ByteValue;
	int16 Integer16Value;
	int32 Integer32Value;
	int64 Integer64Value;

	float FloatValue;
	double DoubleValue;

	FString StringValue;
	FChar CharValue;

	bool BooleanValue;
};

USTRUCT(BlueprintType)
struct POSTGRESQL_API FPQRow
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FString, FPQField> Fields;

	FPQRow() = default;
	FPQRow(pqxx::row* Row);

	FString ToString() const;
};

USTRUCT(BlueprintType)
struct POSTGRESQL_API FPQPreparedQuery
{
	GENERATED_BODY()

public:
	FPQPreparedQuery() = default;
	FPQPreparedQuery(const FString& SQL, const TArray<FPQField>& Args);

	FORCEINLINE const FString& GetSQL() const { return SQL; }
	FORCEINLINE const TArray<FPQField>& GetArgs() const { return Args; }

	FString ToString() const;
	
private:
	FString SQL;
	TArray<FPQField> Args;
};
