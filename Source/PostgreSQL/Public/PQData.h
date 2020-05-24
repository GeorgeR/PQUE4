#pragma once

#include "CoreMinimal.h"
#include "Containers/Union.h"

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
	FPQField()
		: DataType(EPQDataType::PQ_Boolean),
		bIsNull(false) { }

	FPQField(EPQDataType DataType)
		: DataType(DataType),
		bIsNull(true) { }

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

	FORCEINLINE bool IsNull() const { return bIsNull; }

	template <typename T>
	T As(const T& DefaultValue) const;

	template <typename T>
	T As() const;

	template <> uint8 As<uint8>(const uint8& DefaultValue) const { return IsNull() ? DefaultValue : ByteValue; }
	template <> uint8 As<uint8>() const { return As<uint8>(0);; }

	template <> int16 As<int16>(const int16& DefaultValue) const { return IsNull() ? DefaultValue : Integer16Value; }
	template <> int16 As<int16>() const { return As<int16>(0); }

	template <> int32 As<int32>(const int32& DefaultValue) const { return IsNull() ? DefaultValue : Integer32Value; }
	template <> int32 As<int32>() const { return As<int32>(0); }

	template <> int64 As<int64>(const int64& DefaultValue) const { return IsNull() ? DefaultValue : Integer64Value; }
	template <> int64 As<int64>() const { return As<int64>(0); }

	template <> float As<float>(const float& DefaultValue) const { return IsNull() ? DefaultValue : FloatValue; }
	template <> float As<float>() const { return As<float>(0.0); }

	template <> double As<double>(const double& DefaultValue) const { return DoubleValue; }
	template <> double As<double>() const { return As<double>(0.0); }

	template <> FString As<FString>(const FString& DefaultValue) const { return IsNull() ? DefaultValue : StringValue; }
	template <> FString As<FString>() const { return As<FString>(TEXT("")); }

	template <> FChar As<FChar>(const FChar& DefaultValue) const { return IsNull() ? DefaultValue : CharValue; }
	template <> FChar As<FChar>() const { return As<FChar>(FChar()); }

	template <> bool As<bool>(const bool& DefaultValue) const { return IsNull() ? DefaultValue : BooleanValue; }
	template <> bool As<bool>() const { return As<bool>(false); }

	FString ToString() const;

private:
	EPQDataType DataType;
	bool bIsNull;
	
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
