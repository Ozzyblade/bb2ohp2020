// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Bounce_Back_2EditorTarget : TargetRules
{
    public Bounce_Back_2EditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
		
		ExtraModuleNames.Add("Bounce_Back_2");
	}
}
