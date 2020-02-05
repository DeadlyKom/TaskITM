using UnrealBuildTool;

public class TaskITM : ModuleRules
{
	public TaskITM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "AIModule"
            });

		PrivateDependencyModuleNames.AddRange(
            new string[] {
            });
	}
}
