#include "PQData.h"

FPQField::FPQField(const uint8& Value)
	: DataType(EPQDataType::PQ_UInt8)
{
	ByteValue = Value;
}

FPQField::FPQField(const int16& Value)
	: DataType(EPQDataType::PQ_Int16)
{
	Integer16Value = Value;
}

FPQField::FPQField(const int32& Value)
	: DataType(EPQDataType::PQ_Int32)
{
	Integer32Value = Value;
}

FPQField::FPQField(const int64& Value)
	: DataType(EPQDataType::PQ_Int64)
{
	Integer64Value = Value;
}

FPQField::FPQField(const float& Value)
	: DataType(EPQDataType::PQ_Float)
{
	FloatValue = Value;
}

FPQField::FPQField(const double& Value)
	: DataType(EPQDataType::PQ_Double)
{
	DoubleValue = Value;
}

FPQField::FPQField(const FString& Value)
	: DataType(EPQDataType::PQ_String)
{
	StringValue = Value;
}

FPQField::FPQField(const FChar& Value)
	: DataType(EPQDataType::PQ_Char)
{
	CharValue = Value;
}

FPQField::FPQField(const bool& Value)
	: DataType(EPQDataType::PQ_Boolean)
{
	BooleanValue = Value;
}

FString FPQField::ToString() const
{
	return FString::Printf(TEXT("Not Implemented"));
}

FPQRow::FPQRow(pqxx::row* Row)
{
	for (auto i = 0; i < (int32)Row->size(); i++)
	{
		auto RowField = (*Row)[i];

		FString ColumnName = FString(ANSI_TO_TCHAR((char*)(*Row)[i].name()));
		FPQField Field;
		
		switch ((*Row)[i].type())
		{
		case 17: // Byte
			Field = FPQField((uint8)RowField.as<int32>());
			break;

		case 21: // Int (2 bit)
			Field = FPQField(RowField.as<int16>());
			break;

		case 23: // Int (4 bit)
			Field = FPQField(RowField.as<int32>());
			break;

		case 20: // Int (8 bit)
			Field = FPQField(RowField.as<int64>());
			break;


		case 700:
			Field = FPQField(RowField.as<float>());
			break;

		case 701:
			Field = FPQField(RowField.as<double>());
			break;


		case 1263: // C_STR
		case 2275:
		case 1043: // VARCHAR
		case 25: // Text
			Field = FPQField(FString(RowField.as<std::string>().c_str()));
			break;

		case 18: // Char
			Field = FPQField(ANSI_TO_TCHAR(RowField.as<std::string>().c_str())[0]);
			break;


		case 16: // Boolean
			Field = FPQField(RowField.as<bool>());
			break;
		}

		Fields.Add(ColumnName, Field);
	}
}

FString FPQRow::ToString() const
{
	return FString::Printf(TEXT("Not Implemented"));
}

FPQPreparedQuery::FPQPreparedQuery(const FString& SQL, const TArray<FPQField>& Args)
	: SQL(SQL),
	Args(Args) { }

FString FPQPreparedQuery::ToString() const
{
	return FString::Printf(TEXT("Not Implemented"));
}
