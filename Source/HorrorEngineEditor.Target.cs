

using UnrealBuildTool;
using System.Collections.Generic;

public class HorrorEngineEditorTarget : TargetRules
{
	public HorrorEngineEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "HorrorEngine" } );
	}
}
