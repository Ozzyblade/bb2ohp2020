// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bounce_Back_2Ball.h"
#include "DarkZone.generated.h"

UCLASS()
class BOUNCE_BACK_2_API ADarkZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADarkZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// AActor interface
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UFUNCTION()
	void OnElapsed();

	// Volume representing the dark zone for triggering events
	UPROPERTY(EditAnywhere)
	UBoxComponent* m_pVolume;
	
	// Container of references to characters inside the volume
	UPROPERTY()
	TArray<ABounce_Back_2Ball*>	m_containedCharacters;

	// Timer handle to pause etc.
	FTimerHandle m_respawnTimer;

	// True if global timer is currently counting down
	bool m_isTimerActive;

	// Setting to false results in no forced respawning
	UPROPERTY(EditAnywhere)
	bool m_isDeadly = true;

	// Time that must elapse before forced respawning is triggered
	UPROPERTY(EditAnywhere)
	float m_respawnTime = 3.0f;
};
