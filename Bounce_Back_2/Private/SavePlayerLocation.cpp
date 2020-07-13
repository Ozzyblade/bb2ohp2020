// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "SavePlayerLocation.h"

USavePlayerLocation::USavePlayerLocation()
{
	// Set the save slot name 
	SaveSlotName = TEXT("PlayerLocations");

	// Set zero as the default location
	LanosLocation = FVector(0, 0, 0);
	SpikeLocation= FVector(0, 0, 0);
	MarbLocation= FVector(0, 0, 0);
	MagnaLocation = FVector(0, 0, 0);
}