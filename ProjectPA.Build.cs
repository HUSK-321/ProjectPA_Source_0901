// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectPA : ModuleRules
{
	public ProjectPA(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "PAWeatherSystem", "Water", "MotionWarping", "PoseSearch", "Niagara" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"NetCore",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayDebugger",
			"GameplayTags",
			"Slate",
            "SlateCore",
            "PhysicsCore",
            "AnimGraphRuntime",
            "MotionWarping",
            "PoseSearch",
            "AnimationModifiers",
            "AnimationBlueprintLibrary",
            "ModelViewViewModel",
            "CommonUI",
		});
		
		// Editor 전용 모듈 제거 (if 조건 추가)
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"UnrealEd",
				"GameplayTagsEditor",
				"AssetTools"
			});
		}
	}
}
