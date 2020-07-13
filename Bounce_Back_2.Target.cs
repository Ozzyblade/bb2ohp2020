// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Bounce_Back_2Target : TargetRules
{
    public Bounce_Back_2Target(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Bounce_Back_2");
	}
}
