// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Screw_Hazard.h"

AScrew_Hazard::AScrew_Hazard()
{
	//AMagneticObject::AMagneticObject();

	
	objType = MagnetType::VE_FORCE_MOVEABLE;
}


void AScrew_Hazard::BeginPlay()
{
	//AMagneticObject::BeginPlay();
	Super::BeginPlay();
	KillZone->Activate(); //activate killzone 
	MagneticZone->Activate();
	KillZone->SetGenerateOverlapEvents(true);
	MagneticZone->SetGenerateOverlapEvents(true);
	UE_LOG(LogTemp, Warning, TEXT("screw obj begin play func"));
}