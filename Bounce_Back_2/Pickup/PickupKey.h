// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/Pickup.h"
#include "PickupKey.generated.h"

/**
 * 
 */
UCLASS()
class BOUNCE_BACK_2_API APickupKey : public APickup
{
	GENERATED_BODY()
public:
	APickupKey();

	void OnCollected() override;

	//dummy root component to aligned everything together
	UPROPERTY()
		USceneComponent* DummyRoot;

	//visual representation of the switch's base
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Base;

	// To be called when a player collides with the puzzle board
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
