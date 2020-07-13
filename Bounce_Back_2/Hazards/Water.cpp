// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Water.h"

AWater::AWater ()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    decal = CreateDefaultSubobject <UDecalComponent> (TEXT ("WaterDecal"));
    decal->SetRelativeRotation (FRotator (90.0f, 0.0f, 0.0f));
    RootComponent = decal;

    // Create box components
    CollisionBox1 = CreateDefaultSubobject <UBoxComponent> (TEXT ("CollisionBoxWater1"));
    CollisionBox1->SetMobility (EComponentMobility::Stationary);
    CollisionBox1->SetupAttachment (RootComponent);

    CollisionBox2 = CreateDefaultSubobject <UBoxComponent> (TEXT ("CollisionBoxWater2"));
    CollisionBox2->SetMobility (EComponentMobility::Stationary);
    CollisionBox2->SetupAttachment (RootComponent);

    CollisionBox3 = CreateDefaultSubobject <UBoxComponent> (TEXT ("CollisionBoxWater3"));
    CollisionBox3->SetMobility (EComponentMobility::Stationary);
    CollisionBox3->SetupAttachment (RootComponent);

	// Init Variables
	speedDampening = 1.f;
}
