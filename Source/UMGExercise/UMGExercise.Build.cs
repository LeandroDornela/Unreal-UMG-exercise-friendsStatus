// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UMGExercise : ModuleRules
{
	public UMGExercise(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
