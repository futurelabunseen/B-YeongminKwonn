// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ArcadeProtoType : ModuleRules
{
	public ArcadeProtoType(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.AddRange(new string[] { "ArcadeProtoType" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "MultiplayerSessions", "InputCore", "EnhancedInput" , "UMG", "NavigationSystem", "AIModule", "GameplayTasks", "Niagara", "OnlineSubsystemSteam", "OnlineSubsystem","MotionWarping" });
	}
}
