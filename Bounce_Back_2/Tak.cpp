// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Tak.h"
#include "Bounce_Back_2GameMode.h"
#include "Hazards/Pencil.h"

ATak::ATak ()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh (TEXT ("/Game/Assets/Meshes/TakModel.TakModel")); check (Mesh.Succeeded ());

    // Create model component for tak (use the model for collisions for now)
    Ball->SetStaticMesh (Mesh.Object);

    Ball->SetRelativeScale3D (FVector (3.0f, 3.0f, 3.0f));
    Ball->SetPhysicsMaxAngularVelocity (1000.0f);
}

// Called when the game starts or when spawned
void ATak::BeginPlay ()
{
    Super::BeginPlay ();

   // gameMode->Tak = this;
}

void ATak::NotifyActorBeginOverlap (AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap (OtherActor);

    // check for Pencil
    {
        // cast actor to APencil
        AStationery_Pencil* const Pencil = Cast <AStationery_Pencil> (OtherActor);

        // if cast is successful
        if (Pencil)
        {
            Kill (false);
        }
    }
}

void ATak::SetupPlayerInputComponent (class UInputComponent* PlayerInputComponent)
{
    // set up gameplay key bindings
    Super::SetupPlayerInputComponent (PlayerInputComponent);

    PlayerInputComponent->BindAction ("Action", IE_Pressed, this, &ATak::ActionPressed);
}

void ATak::ActionPressed ()
{
    ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode> (UGameplayStatics::GetGameMode (GetWorld ()));
    if (GameMode)
    {
        if (!GameMode->GetIsGameplayEnabled ()) return;
    }
}
