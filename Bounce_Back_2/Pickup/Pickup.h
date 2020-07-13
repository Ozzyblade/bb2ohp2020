// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BOUNCE_BACK_2_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

    // return whether the pickup is active
    UFUNCTION (BlueprintPure, Category = "Pickup")
    bool IsActive () const;

    // allow other classes change whether pickup is active
    UFUNCTION (BlueprintCallable, Category = "Pickup")
    void SetActive (bool NewPickupState);

    // function to call when the pickup os collected
    virtual void OnCollected ();

protected:

    // true when the pickup can be used, false when deactivated
    bool bIsActive;
};
