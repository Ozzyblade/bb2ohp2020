// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Switch.h"
#include "MovingPlatform.generated.h"

UCLASS()
class BOUNCE_BACK_2_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex);

	//dummy root component to aligned everything together
	UPROPERTY()
		USceneComponent* DummyRoot;

	//moving dummy root component to move everything accurately
	UPROPERTY()
		USceneComponent* MovingRoot;

	//collision box to activate platform movement
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

	//static mesh that represents the platform
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Platform;

	//a mesh to represent the start point
	//note not visible in game, or collidable
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StartPoint;

	//a mesh to represent the end point
	//note not visible in game, or collidable
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* EndPoint;

	//if the object moves vertically or horizontally
	UPROPERTY(EditAnywhere)
		bool VerticalMovement;

	/*
	-- will be added on later version
	//if the object moves in both direction
	UPROPERTY(EditAnywhere)
	bool SimultaneousMovement;
	*/

	//how long it will take the platform to move
	UPROPERTY(EditAnywhere)
		float movementDuration;

	//how long the platform will wait before moving again continous
	UPROPERTY(EditAnywhere)
		float waitDuration;

	//if the platform is in continuos movement 
	//or triggerable by the player
	UPROPERTY(EditAnywhere)
		bool needsActivating;

	//if the platform is in continuos movement 
	//or triggerable by the player
	UPROPERTY(EditAnywhere)
		bool continuous;

	//if the platform starts at the start first 
	UPROPERTY(EditAnywhere)
		bool atStart;

	//switch to return the object to start point
	UPROPERTY(EditAnywhere)
		ASwitch* StartSwitch;

	//switch to return the object to end point
	UPROPERTY(EditAnywhere)
		ASwitch* EndSwitch;

private:
	//functionality for timer end for non-continuous platforms
	void TimerEnd();
	//functionaloity for timer end for continuos platforms
	void TimerEndContinuous();

	void MoveToStart(FLatentActionInfo LatentInfo);
	void MoveToEnd(FLatentActionInfo LatentInfo);

	FTimerHandle moveTimer;

	bool canMove;
	bool Moving;
	bool ifActivated;
};
