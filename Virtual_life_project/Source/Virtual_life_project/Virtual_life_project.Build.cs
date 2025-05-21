// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class Virtual_life_project : ModuleRules
{
	public Virtual_life_project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Sockets", "Networking", "UMG", "Slate", "SlateCore", "Json", "JsonUtilities", "AudioCapture", "AudioMixer", "AudioMixerCore", "AudioCaptureCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "AudioCapture" });

        string OpusPath = Path.Combine(ModuleDirectory, "ThirdParty/");

        PublicIncludePaths.Add(OpusPath);
        PublicAdditionalLibraries.Add(Path.Combine(OpusPath, "opus.lib"));
    }
}
