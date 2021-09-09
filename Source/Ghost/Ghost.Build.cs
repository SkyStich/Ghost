// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ghost : ModuleRules
{
	public Ghost(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay",
			"AIModule",
			"NavigationSystem",
			"GameplayTasks"
		});
	}
}
