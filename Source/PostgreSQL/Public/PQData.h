#pragma once

#include "CoreMinimal.h"

#include <pqxx/pqxx>

#include "PQData.generated.h"

UENUM(BlueprintType)
enum class EPQDataType : uint8
{
	PQ_UInt8		UMETA(DisplayName = "Byte"),
	PQ_Int32		UMETA(DisplayName = "Integer"),
	PQ_Int64		UMETA(DisplayName = "Integer64"),

	PQ_Float		UMETA(DisplayName = "Float"),
	PQ_Double		UMETA(DisplayName = "Double"),

	PQ_String		UMETA(DisplayName = "String"),
	PQ_Char			UMETA(DisplayName = "Char"),

	PQ_Boolean		UMETA(DisplayName = "Boolean")
};

USTRUCT(BlueprintType)
struct POSTGRESQL_API FPQCell
{
	GENERATED_BODY()

public:
	FPQCell() { }

	//template <typename T>
	//FPQCell(T Value);

	FORCEINLINE EPQDataType GetDataType() const { return DataType; }

	template <typename T>
	T Get() const;

	template <> uint8 Get<uint8>() const { return ByteValue; }
	template <> int32 Get<int32>() const { return IntegerValue; }
	template <> int64 Get<int64>() const { return Integer64Value; }

private:
	EPQDataType DataType;

	uint8 ByteValue;
	int32 IntegerValue;
	int64 Integer64Value;

	float FloatValue;
	double DoubleValue;

	FString StringValue;
	FChar CharValue;

	bool BooleanValue;
};

//template <>	FPQCell::FPQCell<uint8>(uint8 Value) : DataType(EPQDataType::PQ_UInt8), ByteValue(Value) { }

USTRUCT(BlueprintType)
struct POSTGRESQL_API FPQRow
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FString, FPQCell> Cells;

	FPQRow() { }
	FPQRow(pqxx::row* Row);
};
