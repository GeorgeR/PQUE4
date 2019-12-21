using UnrealBuildTool;
using System.IO;

public class PostgreSQL : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../ThirdParty/")); }
    }

    public PostgreSQL(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        if (Target.Version.MinorVersion <= 19)
        {
            PublicIncludePaths.AddRange(
			    new string[] {
				    "PostgreSQL/Public"
			    });

		    PrivateIncludePaths.AddRange(
			    new string[] {
				    "PostgreSQL/Private"
			    });
        }

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",

                "DbPlus",
            });

        //PrivateDependencyModuleNames

        bEnableExceptions = true;
        LoadPQXX(Target);

        if (Target.bBuildEditor == true)
	        PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "ContentBrowser" });
    }

    private bool LoadPQXX(ReadOnlyTargetRules Target)
    {
        bool bIsLibrarySupported = false;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            bIsLibrarySupported = true;

            var PQXXLibrariesPath = Path.Combine(ThirdPartyPath, "pqxx", "lib", "Win64");
            var PQLibrariesPath = Path.Combine(ThirdPartyPath, "pq", "lib", "Win64");

            PublicAdditionalLibraries.Add(Path.Combine(PQLibrariesPath, "postgres.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(PQLibrariesPath, "libpq.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(PQXXLibrariesPath, "libpqxx_static.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(PQLibrariesPath, "libcrypto.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(PQLibrariesPath, "libssl.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            bIsLibrarySupported = true;

            var PQXXLibrariesPath = Path.Combine(ThirdPartyPath, "pqxx", "lib", "Linux");
            var PQLibrariesPath = Path.Combine(ThirdPartyPath, "pq", "lib", "Linux");

            PublicAdditionalLibraries.Add(Path.Combine(PQXXLibrariesPath, "libpqxx.a"));
            PublicAdditionalLibraries.Add(Path.Combine(PQLibrariesPath, "libpq.a"));
        }

        if (bIsLibrarySupported)
        {
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "pqxx", "inc"));
        }

        return bIsLibrarySupported;
    }
}
