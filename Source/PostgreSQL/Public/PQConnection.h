#pragma once

#include "CoreMinimal.h"

//class FPQWork;

class POSTGRESQL_API FPQConnection
{
public:
	FPQConnection(FString& InConnectionString);
	
	//TSharedRef<FPQWork> BeginTransaction();

private:
	FString ConnectionString;
	//pqxx::connection Connection;
};