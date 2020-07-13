// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Pickup.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // all pickups start active
    bIsActive = true;
}

// return whether the pickup is active
bool APickup::IsActive () const
{
    return bIsActive;
}

// allow other classes change whether pickup is active
void APickup::SetActive (bool NewPickupState)
{
    bIsActive = NewPickupState;
}

void APickup::OnCollected ()
{
    // log debug message
    FString const PickupDebugString = GetName ();

    UE_LOG (LogClass, Log, TEXT ("You have collected %s"), *PickupDebugString);
}
