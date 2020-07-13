// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "PickupPuzzle.h"
#include "PuzzleBoard.h"
#include "Bounce_Back_2Ball.h"

// Sets default values for this actor's properties
APickupPuzzle::APickupPuzzle ()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // create the static mesh component
    Mesh = CreateDefaultSubobject <UStaticMeshComponent> (TEXT ("Mesh"));
    Mesh->BodyInstance.SetCollisionProfileName ("Trigger");
    RootComponent = Mesh;

    SetActorRelativeRotation (rotation);

    rotation.Pitch = 90.0f;
    rotation.Roll = 0.0f;
    rotation.Yaw = 0.0f;

	PickupPuzzleAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("PickupPuzzleAudio"));
	PickupPuzzleAudio->SetupAttachment(RootComponent);
	PickupPuzzleAudio->bAutoActivate = false;
}

void APickupPuzzle::OnCollected ()
{
    // use base pickup behaviour first
    Super::OnCollected ();

    // destroy battery
    Destroy ();
}

PuzzleShape APickupPuzzle::GetPuzzleShape ()
{
    return shape;
}

// Called every frame	
void APickupPuzzle::Tick (float DeltaSeconds)
{
    Super::Tick (DeltaSeconds); // Call parent class tick function

    rotation.Yaw += DeltaSeconds * 60.0f;

    SetActorRelativeRotation (rotation);
}

void APickupPuzzle::NotifyActorBeginOverlap(AActor* pOtherActor)
{
	Super::NotifyActorBeginOverlap(pOtherActor);

	if (!IsPendingKill() && IsActive())
	{
		// check if the other actor is one of the characters
		ABounce_Back_2Ball* const pCharacter = Cast <ABounce_Back_2Ball>(pOtherActor);
		if (pCharacter != nullptr)
		{
			OnCollected();

			//tell the puzzle board which character owns the piece now
			m_pBoard->setPieceOwner(shape, pCharacter);

			if (PickupPuzzleAudio->Sound != nullptr)
			{
				PickupPuzzleAudio->Play();
			}

			// deactivate pickup
			SetActive(false);
			Mesh->SetVisibility(false);
		}
	}
}
