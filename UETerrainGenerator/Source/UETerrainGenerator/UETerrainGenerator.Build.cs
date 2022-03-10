// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UETerrainGenerator : ModuleRules
{
	public UETerrainGenerator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",
		"Voxel"
		});
	}
}
