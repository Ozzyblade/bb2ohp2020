// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "RotatorObject.h"
#include "TriggerButton.h"

// Sets default values
ARotatorObject::ARotatorObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ControlType = ControlType::VE_Button;
	RotationType = RotationType::VE_Linear;
	StarterRotation = FRotator(0, 0, 0);
	RotationAngle = FVector(0, 0, 0);
	PointRotationAmount = FRotator(0, 0, 0);
	AimedVector = FRotator(0, 0, 0);
}

// Called when the game starts or when spawned
void ARotatorObject::BeginPlay()
{
	Super::BeginPlay();

	StarterRotation = GetActorRotation();
	AimedVector = FRotator(StarterRotation.Pitch + PointRotationAmount.Pitch, StarterRotation.Yaw + PointRotationAmount.Yaw, StarterRotation.Roll+ PointRotationAmount.Roll);
}

// Called every frame
void ARotatorObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float X = GetActorRotation().Pitch + RotationAngle.X;
	float Y = GetActorRotation().Yaw + RotationAngle.Y;
	float Z = GetActorRotation().Roll + RotationAngle.Z;

	FRotator Test = FRotator(X, Y, Z);

	if (ControlType == ControlType::VE_Button)
	{
		if (RotationType == RotationType::VE_Linear)
		{
			if (TriggerButton->GetPressed())
				SetActorRotation(Test);
		}

		if (RotationType == RotationType::VE_Point)
		{
			if (TriggerButton->GetPressed())
			{
				SetActorRotation(AimedVector);
			}
			else
			{
				SetActorRotation(StarterRotation);
			}
		}
	}
}





