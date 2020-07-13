// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Pen.h"


// Sets default values
APen::APen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create mesh component
	static ConstructorHelpers::FObjectFinder <UStaticMesh> PenMesh(TEXT("/Game/Assets/BB8_Assets/BB8_Meshes/BB2_Ballpoint_Pen_01_Asset.BB2_Ballpoint_Pen_01_Asset")); check(PenMesh.Succeeded());
	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pen"));
	m_pMesh->SetStaticMesh(PenMesh.Object);
	RootComponent = m_pMesh;

	m_pCollisionBox = CreateDefaultSubobject <UBoxComponent>(TEXT("CollisionBoxPen"));;
	m_pCollisionBox->SetRelativeLocationAndRotation(FVector(0.0f, -16.0f, 1030.0f), FRotator(0.0f, 0.0f, 0.0f));
	m_pCollisionBox->SetBoxExtent(FVector(20.0f, 20.0f, 30.0f), true);
	m_pCollisionBox->SetupAttachment(RootComponent);
}


