// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Pencil.h"

// Sets default values
AStationery_Pencil::AStationery_Pencil()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create mesh component
	static ConstructorHelpers::FObjectFinder <UStaticMesh> PencilMesh(TEXT("/Game/Assets/Meshes/Stationery/Stationery_Pencil.Stationery_Pencil")); check(PencilMesh.Succeeded());
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pencil"));
	Mesh->SetStaticMesh(PencilMesh.Object);
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder <UStaticMesh> BrokenMesh(TEXT("/Game/Assets/Meshes/Stationery/Stationery_PencilBroken.Stationery_PencilBroken")); check(BrokenMesh.Succeeded());
	m_pBrokenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Broken Pencil"));
	m_pBrokenMesh->SetStaticMesh(BrokenMesh.Object);
	m_pBrokenMesh->SetRelativeLocation(FVector(-4.0f, -6.0f, 0.0f));
	m_pBrokenMesh->SetVisibility(false);
	m_pBrokenMesh->SetupAttachment(RootComponent);


	CollisionBox = CreateDefaultSubobject <UBoxComponent>(TEXT("CollisionBoxPencil"));;
	CollisionBox->SetRelativeLocationAndRotation(FVector(-3.5f, -6.2f, 222.0f), FRotator(0.0f, 0.0f, 0.0f));
	CollisionBox->SetBoxExtent(FVector(6.0f, 6.0f, 13.5f), true);
	CollisionBox->SetupAttachment(RootComponent);
}

bool AStationery_Pencil::isBlunted()
{
	return m_blunt;
}

//Pencil no longer a hazard because of Spike
void AStationery_Pencil::blunted()
{
	CollisionBox->DestroyComponent();
	m_blunt = true;
	Mesh->DestroyComponent();
	m_pBrokenMesh->SetVisibility(true);
	RootComponent = m_pBrokenMesh;
}