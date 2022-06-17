// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FirstPersonHorrorEditorTarget : TargetRules
{
	public FirstPersonHorrorEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("FirstPersonHorror");

		bUseUnityBuild = false;
		bUsePCHFiles = false;
	}
}
