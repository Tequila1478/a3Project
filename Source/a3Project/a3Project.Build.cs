// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class a3Project : ModuleRules
{
	public a3Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
