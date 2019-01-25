#include "IPostgreSQLModule.h"

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Stats/Stats.h"
#include "Stats/Stats2.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPQ, VeryVerbose, All);

DECLARE_STATS_GROUP(TEXT("PostgreSQL"), STATGROUP_PQ, STATCAT_Advanced);

class FPostgreSQLModule
	: public IPostgreSQLModule
{
	public:
	
};
