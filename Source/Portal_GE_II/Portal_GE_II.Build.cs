// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Portal_GE_II : ModuleRules
{
	public Portal_GE_II(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
