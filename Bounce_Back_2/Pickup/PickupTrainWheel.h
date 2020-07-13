// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/Pickup.h"
#include "PickupTrainWheel.generated.h"

UCLASS()
class BOUNCE_BACK_2_API APickupTrainWheel : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupTrainWheel();

	// To be called when a player collides with the puzzle board
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// override WasCollected - useImplementation because it's a BlueprintNateEvent
	void OnCollected() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// static mesh to represent the pickup in the world
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent *PickupMesh;

	FRotator rotation;

	/** Audio component for the pickup of a puzzle piece */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle Piece", meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* PickupPuzzleAudio;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
