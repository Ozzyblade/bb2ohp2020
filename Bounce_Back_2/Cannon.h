// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

class UStaticMeshComponent;
class ALanos;

UCLASS()
class BOUNCE_BACK_2_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	float CannonImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	float CannonHoldTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	bool bMakeInvisible;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CannonMesh;

	// This is done as a mesh to provide a physical representation of the location of the muzzle
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CannonMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	ALanos* Lanos;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void FireLanos();
	FTimerHandle TimerHandle_ButtonTimeLength;
};

