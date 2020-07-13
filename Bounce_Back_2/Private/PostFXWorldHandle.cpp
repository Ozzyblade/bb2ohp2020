// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "PostFXWorldHandle.h"
#include "Components/PostProcessComponent.h"


// Sets default values
APostFXWorldHandle::APostFXWorldHandle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the post fx component
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostFXComponent"));
	PostProcessSettings.bOverride_ColorSaturation = true;

	
}

// Called when the game starts or when spawned
void APostFXWorldHandle::BeginPlay()
{
	Super::BeginPlay();
	PostProcessSettings.ColorSaturation.Set(1.0f, 1.0f, 1.0f, 1.f);
	PostProcessComponent->Settings = PostProcessSettings;
}

// Called every frame
void APostFXWorldHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void APostFXWorldHandle::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
//{
//}

void APostFXWorldHandle::ActivateFX()
{
	this->PostProcessSettings.ColorSaturation.Set(1.0f, 1.0f, 1.0f, 0.1f);
	this->PostProcessComponent->Settings = PostProcessSettings;
}

void APostFXWorldHandle::DeactivateFX()
{
	this->PostProcessSettings.ColorSaturation.Set(1.0f, 1.0f, 1.0f, 1.f);
	this->PostProcessComponent->Settings = PostProcessSettings;
}