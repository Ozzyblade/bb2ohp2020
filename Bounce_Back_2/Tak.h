// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Bounce_Back_2Ball.h"
#include "Tak.generated.h"

/**
 * 
 */
UCLASS()
class BOUNCE_BACK_2_API ATak : public ABounce_Back_2Ball
{
	GENERATED_BODY()
	
public:
	ATak();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // AActor interface
    virtual void NotifyActorBeginOverlap (AActor* OtherActor) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
protected:
	/** Handle the action. */
	void ActionPressed();
};
