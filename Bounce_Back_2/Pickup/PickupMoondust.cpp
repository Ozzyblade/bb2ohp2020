// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "PickupMoondust.h"

// Sets default values for this actor's properties
APickupMoondust::APickupMoondust ()
{
    // create the skeletal mesh component
    static ConstructorHelpers::FObjectFinder <USkeletalMesh> SkeletalMesh (TEXT ("/Game/Assets/Meshes/Pickup/Pickup_Moondust.Pickup_Moondust")); check (SkeletalMesh.Succeeded ());
    static ConstructorHelpers::FObjectFinder <UAnimationAsset> Anim (TEXT ("/Game/Assets/Animations/Pickup/Pickup_Moondust_Idle.Pickup_Moondust_Idle")); check (Anim.Succeeded ());
    Mesh = CreateDefaultSubobject <USkeletalMeshComponent> (TEXT ("Mesh"));
    Mesh->SetSkeletalMesh (SkeletalMesh.Object);
    Mesh->SetAnimationMode (EAnimationMode::AnimationSingleNode);
    Mesh->SetAnimation (Anim.Object);
    RootComponent = Mesh;

    // create the particle system component
    static ConstructorHelpers::FObjectFinder <UParticleSystem> ParticleSystem (TEXT ("/Game/Assets/Particles/Pickup/Pickup_Moondust.Pickup_Moondust")); check (ParticleSystem.Succeeded ());
    Particles = CreateDefaultSubobject <UParticleSystemComponent> (TEXT ("Particles"));
    Particles->SetTemplate (ParticleSystem.Object);
    Particles->SetupAttachment (RootComponent);

    // create the particle system component
    DetectionArea = CreateDefaultSubobject <UCapsuleComponent> (TEXT ("DetectionArea"));
    DetectionArea->SetRelativeLocation (FVector (0.0f, 0.0f, 120.0f));
    DetectionArea->SetCapsuleHalfHeight (120.0f);
    DetectionArea->SetCapsuleRadius (35.0f);
    DetectionArea->SetupAttachment (RootComponent);

    //the base moon dust value
    MoondustValue = 1;
}

void APickupMoondust::OnCollected ()
{
    // use base pickup behaviour first
    Super::OnCollected ();

    // destroy battery
    Destroy ();
}

// report moondust value
int32 APickupMoondust::GetMoondustValue () const
{
    return MoondustValue;
}
