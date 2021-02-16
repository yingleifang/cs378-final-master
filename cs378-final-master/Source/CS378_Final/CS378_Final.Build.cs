// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CS378_Final : ModuleRules
{
	public CS378_Final(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ProceduralMeshComponent", "RuntimeMeshComponent", "SimplexNoise", "UMG", "Slate", "SlateCore", "AIModule" });

		PrivateDependencyModuleNames.AddRange(new string[] { "SimplexNoise" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
