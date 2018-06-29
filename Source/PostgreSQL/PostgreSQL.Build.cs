using UnrealBuildTool;
using System.IO;

public class PostgreSQL : ModuleRules
{
	public PostgreSQL(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"PostgreSQL/Public"
			});

		PrivateIncludePaths.AddRange(
			new string[] {
				"PostgreSQL/Private"
			});

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "RenderCore",
                "ShaderCore",
                "RHI"
            });

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty/libpqxx/include"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty/libpqxx/src"));

        if (Target.bBuildEditor == true)
	        PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "ContentBrowser" });
    }
}