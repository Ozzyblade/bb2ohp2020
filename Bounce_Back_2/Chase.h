// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chase.generated.h"

class ADarkZone;

UCLASS()
class BOUNCE_BACK_2_API AChase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChase();

	//volume to trigger start of chase
	UPROPERTY(EditAnywhere)
		UBoxComponent* StartTriggerVolume;

	//volume to trigger end of chase
	UPROPERTY(EditAnywhere)
		UBoxComponent* EndTriggerVolume;

	//dark zones that appear and spread over time
	UPROPERTY(EditAnywhere)
		TArray<ADarkZone*> DarkZones;

	//scales each DarkZone should be before manipulating the next
	UPROPERTY(EditAnywhere)
		TArray<FVector> DarkZoneTargetScales;

	//position each DarkZone should be before manipulating the next
	UPROPERTY(EditAnywhere)
		TArray<FVector> DarkZoneTargetPositions;

	//time for each dark zone to reach target scale
	UPROPERTY(EditAnywhere)
		TArray<float> DarkZoneTimings;

	//lights to be disabled during chase
	UPROPERTY()
		TArray<UPointLightComponent*> Lights;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void reset();

	UFUNCTION()
		void OnOverlapStart(UPrimitiveComponent* pOverlappedComponent, AActor* pOtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* pOverlappedComponent, AActor* pOtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	//only one character is chased
	class ABounce_Back_2Ball* m_pChasedCharacter;

	TArray<FVector> m_darkZoneStartingScales;
	TArray<FVector> m_darkZoneStartingPositions;

	int m_progress;
	float m_remainingTimeForStep;
};
