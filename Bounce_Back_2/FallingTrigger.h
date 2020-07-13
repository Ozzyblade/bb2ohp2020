// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "FallingObject.h"
#include "FallingTrigger.generated.h"


/**
 * 
 */
UCLASS()
class BOUNCE_BACK_2_API AFallingTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
	
public:

	AFallingTrigger();

	// AActor interface
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	

	UPROPERTY(EditAnywhere, Category = ObjectToFall)
		AFallingObject* m_ptargetObject;
};
