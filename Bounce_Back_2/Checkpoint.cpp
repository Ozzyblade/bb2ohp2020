// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Checkpoint.h"

ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // create the skeletal mesh component
    static ConstructorHelpers::FObjectFinder <USkeletalMesh> SkeletalMesh (TEXT ("/Game/Assets/Meshes/Checkpoint/RespawnArea_SkeletalMesh.RespawnArea_SkeletalMesh")); check (SkeletalMesh.Succeeded ());
    Mesh = CreateDefaultSubobject <USkeletalMeshComponent> (TEXT ("Mesh"));
    Mesh->SetSkeletalMesh (SkeletalMesh.Object);
    RootComponent = Mesh;
    
    // Create detection area
    DetectionArea = CreateDefaultSubobject <UBoxComponent> (TEXT ("DetectionArea"));
    //DetectionArea->SetMobility (EComponentMobility::Static); // screws with SetRelativeLocation for some unknown reason
    DetectionArea->SetRelativeLocation (FVector (0.0f, 30.0f, 60.0f));
    DetectionArea->SetBoxExtent (FVector (715.0f, 340.0f, 60.0f));
    DetectionArea->SetupAttachment (RootComponent);

    // set variables
    SpawnOffsets.Emplace (0.0f, 0.0f, 0.0f); // middle
    SpawnOffsets.Emplace (5.0f, 0.0f, 5.0f); // top right
    SpawnOffsets.Emplace (5.0f, 0.0f, -5.0f); // top left
    SpawnOffsets.Emplace (-5.0f, 0.0f, 5.0f); // bottom right
    SpawnOffsets.Emplace (-5.0f, 0.0f, -5.0f); // bottom left
}

// get a viable spawn location, that is clear of any overlapping objects
void ACheckpoint::GetSpawnPoint (FVector & loc, FRotator & rot)
{
    FVector const offset (0.0f); // extra offset to lift character off the floor, based on the origin of the mesh
    loc =  GetActorLocation () + offset + SpawnOffsets [0];

    rot = GetActorRotation ();
}
