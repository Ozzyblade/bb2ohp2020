// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Bounce_Back_2 : ModuleRules
{
	public Bounce_Back_2(ReadOnlyTargetRules Target) : base (Target)
	{
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "MediaAssets", "NavigationSystem" });

        PublicDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
