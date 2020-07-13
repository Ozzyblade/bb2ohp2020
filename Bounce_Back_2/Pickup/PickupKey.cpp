// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Bounce_Back_2Ball.h"
#include "PickupKey.h"

APickupKey::APickupKey()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	DummyRoot->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	DummyRoot->RelativeRotation = FRotator(0.f, 180.f, 0.f);
	RootComponent = DummyRoot;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMesh(TEXT("/Game/Assets/BB8_Assets/BB8_Meshes/BB2_Collectable_Key_Asset.BB2_Collectable_Key_Asset")); check(BaseMesh.Succeeded());
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KEY"));
	Base->SetStaticMesh(BaseMesh.Object);
	Base->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Base->SetupAttachment(RootComponent);
}

void APickupKey::OnCollected()
{
	// use base pickup behaviour first
	Super::OnCollected();

	// destroy battery
	Destroy();
}

void APickupKey::NotifyActorBeginOverlap(AActor* pOtherActor)
{
	if (!IsPendingKill() && IsActive())
	{
		// check if the other actor is one of the characters
		ABounce_Back_2Ball* const pCharacter = Cast <ABounce_Back_2Ball>(pOtherActor);
		if (pCharacter != nullptr)
		{
			OnCollected();
			pCharacter->SetHasKey();

			// deactivate pickup
			SetActive(false);
			Base->SetVisibility(false);
		}
	}
}