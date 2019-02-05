#include "PQConnectionString.h"

FPQConnectionString::FPQConnectionString(
	const FString& DatabaseName,
	const FString& User,
	const FString& Password,
	const FString& Address,
	int32 Port /*= 5432*/)
			: DatabaseName(DatabaseName),
			User(User),
			Password(Password),
			Address(Address),
			Port(Port) { }

FString FPQConnectionString::ToString() const
{
	return FString::Printf(TEXT("dbname=%s user=%s password=%s host=%s port=%i"), *DatabaseName, *User, *Password, *Address, Port);
}
