// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "FloorHazard.h"

// Sets default values
AFloorHazard::AFloorHazard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	decal = CreateDefaultSubobject <UDecalComponent>(TEXT("Decal"));
	decal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	RootComponent = decal;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetMobility(EComponentMobility::Stationary);
	CollisionBox->SetupAttachment(RootComponent);

	speedDampening = 1.f;

}

float AFloorHazard::GetDampening()
{
	return speedDampening;
}

class UPhysicalMaterial* AFloorHazard::GetMaterial()
{
	return Material;
}

