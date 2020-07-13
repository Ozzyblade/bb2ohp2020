// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Block.h"

// Sets default values
ABlock::ABlock ()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create mesh component for the model
    Mesh = CreateDefaultSubobject <UStaticMeshComponent> (TEXT ("Mesh"));
    Mesh->SetRelativeScale3D (FVector (3.0f));
    Mesh->SetSimulatePhysics (true);
    Mesh->SetEnableGravity (true);
    RootComponent = Mesh;

    // set default variable values
    KillTime = 4.0f;
    FlashFrequency = 5.0f;
    FlashOffset = 0.5f;

    ActualMass = 1.0f;

    IsAboutToDie = false;
    TimeSinceKill = 0.0f;
    IsDead = false;

    disable ();
}

void ABlock::Tick (float DeltaSeconds)
{
    Super::Tick (DeltaSeconds);

    if (IsAboutToDie)
    {
        TimeSinceKill += DeltaSeconds;
        if (TimeSinceKill < KillTime)
        {
            // make it flash
            float const Flash = sinf (powf (TimeSinceKill, 2.0f) * FlashFrequency) + FlashOffset;
            Mesh->SetVisibility (Flash > 0.0f);
        }
        else
        {
            IsDead = true;

            Mesh->SetSimulatePhysics (false);
            Mesh->SetEnableGravity (false);
            Mesh->SetVisibility (false);
            Mesh->BodyInstance.SetResponseToAllChannels (ECR_Ignore);
        }
    }
}

void ABlock::enable  ()
{
    if (IsDead) return;

    Mesh->SetMassOverrideInKg (NAME_None, ActualMass, true);
}

void ABlock::disable ()
{
    if (IsDead) return;

    Mesh->SetMassOverrideInKg (NAME_None, 1000000.0f, true);
}

bool ABlock::GetIsAboutToDie () const
{
    return IsAboutToDie;
}

void ABlock::Kill ()
{
    disable ();

    IsAboutToDie = true;
}
