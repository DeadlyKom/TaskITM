using UnrealBuildTool;
using System.Collections.Generic;

public class TaskITMEditorTarget : TargetRules
{
	public TaskITMEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(
            new string[] {
                "TaskITM"
            });
	}
}
