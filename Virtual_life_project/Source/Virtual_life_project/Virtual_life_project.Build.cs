// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Virtual_life_project : ModuleRules
{
	public Virtual_life_project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
