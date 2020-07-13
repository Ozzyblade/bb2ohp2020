// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "TriggerButton.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "GameFramework/PlayerController.h"


// Sets default values
ATriggerButton::ATriggerButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh component for the model
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	RootComponent = MeshComponent;

	/** Defaults */
	bIsPressed = false;
	bWeightOn = false;
	StartLocalVector = FVector(0, 0, 0);
	LoweredVector = FVector(0, 0, 0);
	ButtonDepth = 30;
	ButtonTime = 3.0f;

	ButtonType = ButtonType::VE_Boolean;
}

// Called when the game starts or when spawned
void ATriggerButton::BeginPlay()
{
	Super::BeginPlay();

	StartLocalVector = GetActorLocation();
	LoweredVector = StartLocalVector;
	LoweredVector.Z -= ButtonDepth;
}

// Called every frame
void ATriggerButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bWeightOn)
		SetActorLocation(LoweredVector);
	else
		SetActorLocation(StartLocalVector);

}

void ATriggerButton::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	bIsPressed = true;
	bWeightOn = true;

	if (ButtonType == ButtonType::VE_Timed)
		GetWorldTimerManager().SetTimer(TimerHandle_ButtonTimeLength, this, &ATriggerButton::RemovePressed, ButtonTime, false);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC != nullptr)
	{
		PC->ClientPlayForceFeedback(FFButtonClick, false, "FF_ButtonClick");
	}
}

void ATriggerButton::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	bWeightOn = false;
	if (ButtonType != ButtonType::VE_Timed)
		RemovePressed();
}

void ATriggerButton::RemovePressed()
{
	bIsPressed = false;

}