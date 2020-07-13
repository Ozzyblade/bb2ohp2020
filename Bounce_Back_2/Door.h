// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Switch.h"
#include "Lock.h"
#include "Door.generated.h"

UCLASS()
class BOUNCE_BACK_2_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//visual representation of the door's archway (static)
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Archway;

	//visual representation of the door (moveable)
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Door;

	//collider for the door, if no switch attached
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

	//reference to switch
	UPROPERTY(EditAnywhere)
		ASwitch* Switch;

	//reference to override switch
	UPROPERTY(EditAnywhere)
		ASwitch* OverrideSwitch;

	//reference to lock 
	UPROPERTY(EditAnywhere)
		ALock* Lock;

	//if the door opens clockwise - if swing open
	UPROPERTY(EditAnywhere)
		bool clockwiseRotation;

	//if true swing open, else raise open
	UPROPERTY(EditAnywhere)
		bool SwingOpen;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	bool Opening;
	bool Closing;
	float RotateValue;


	FRotator DoorRotation;
	FVector DoorLocation;

	FVector InitialLocation;
	FVector FinalLocation;
};
