// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lock.generated.h"

UCLASS()
class BOUNCE_BACK_2_API ALock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALock();

	//dummy root component to aligned everything together
	UPROPERTY()
		USceneComponent* DummyRoot;

	//visual representation of the lock
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Lock;

	//visual representation of the lock's bar
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Poles;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenLock();
	bool ReadyToOpen();
private:
	bool locked;
	float timer;
	bool readyToOpen;
};
