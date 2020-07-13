// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SavePlayerLocation.generated.h"


/**
 * 
 */
UCLASS()
class BOUNCE_BACK_2_API USavePlayerLocation : public USaveGame
{
	GENERATED_BODY()

public:

	USavePlayerLocation();

	// The 3 locations of the player characters
	UPROPERTY(EditAnywhere, Category = Basic)
	FVector LanosLocation;

	UPROPERTY(EditAnywhere, Category = Basic)
	FVector SpikeLocation;

	UPROPERTY(EditAnywhere, Category = Basic)
	FVector MarbLocation;

	UPROPERTY(EditAnywhere, Category = Basic)
	FVector MagnaLocation;

	UPROPERTY(EditAnywhere, Category = Basic)
	FString SaveSlotName;

	
};
