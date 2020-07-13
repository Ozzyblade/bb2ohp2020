// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "LevelSwitch.generated.h"

/**
 * 
 */
UCLASS()
class BOUNCE_BACK_2_API ALevelSwitch : public ATriggerBox
{
	GENERATED_BODY()

public:
	ALevelSwitch();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void OnOverlapBegin(class AActor* actor, class AActor* other);
	//UFUNCTION()
	//	void OnOverlapEnd(class AActor* actor, class AActor* other);

	UPROPERTY(EditAnywhere)
		class AActor* SpecificActor;

	UPROPERTY(EditAnywhere)
		FName LevelToLoad;
	
};
