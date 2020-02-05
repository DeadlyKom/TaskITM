using UnrealBuildTool;
using System.Collections.Generic;

public class TaskITMTarget : TargetRules
{
	public TaskITMTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(
            new string[] {
                "TaskITM"
            });
	}
}
