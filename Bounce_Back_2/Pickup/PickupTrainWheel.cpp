// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "PickupTrainWheel.h"
#include "Bounce_Back_2Ball.h"

// Sets default values
APickupTrainWheel::APickupTrainWheel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create the static mesh component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WheelMesh(TEXT("/Game/Assets/BB8_Assets/BB8_Meshes/BB2_Collectable_Wheel_Asset.BB2_Collectable_Wheel_Asset")); check(WheelMesh.Succeeded());
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WHEEL PIECE"));
	PickupMesh->SetStaticMesh(WheelMesh.Object);
	PickupMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	PickupMesh->SetupAttachment(RootComponent);

	SetActorRelativeRotation(rotation);

	rotation.Pitch = 90.0f;
	rotation.Roll = 0.0f;
	rotation.Yaw = 0.0f;

	PickupPuzzleAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("PickupPuzzleAudio"));
	PickupPuzzleAudio->SetupAttachment(RootComponent);
	PickupPuzzleAudio->bAutoActivate = false;

}

void APickupTrainWheel::OnCollected()
{
	// use base pickup behaviour first
	Super::OnCollected();

	// destroy battery
	Destroy();
}


// Called when the game starts or when spawned
void APickupTrainWheel::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APickupTrainWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	rotation.Yaw += DeltaTime * 60.0f;

	SetActorRelativeRotation(rotation);
}

void APickupTrainWheel::NotifyActorBeginOverlap(AActor* pOtherActor)
{
	Super::NotifyActorBeginOverlap(pOtherActor);

	if (!IsPendingKill() && IsActive())
	{
		// check if the other actor is one of the characters
		ABounce_Back_2Ball* const pCharacter = Cast <ABounce_Back_2Ball>(pOtherActor);
		if (pCharacter != nullptr)
		{
			OnCollected();
			pCharacter->SetHasWheel();

			if (PickupPuzzleAudio->Sound != nullptr)
			{
				PickupPuzzleAudio->Play();
			}

			// deactivate pickup
			SetActive(false);
			PickupMesh->SetVisibility(false);
		}
	}
}

