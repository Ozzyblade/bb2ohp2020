// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "SlipperyHazard.h"

ASlipperyHazard::ASlipperyHazard()
{
	// set decal
	static ConstructorHelpers::FObjectFinder <UMaterialInstance> material(TEXT("/Game/Assets/Materials/PuddleInstances/WaterSpill3_D_Inst.WaterSpill3_D_Inst")); check(material.Succeeded());
	decal->SetDecalMaterial(material.Object);

	// set collision
	//CollisionBox->SetRelativeLocationAndRotation(FVector(5.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	CollisionBox->SetBoxExtent(FVector(10.f, 250.f, 500.f), true);


	speedDampening = 1.2f;
}