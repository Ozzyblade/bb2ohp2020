// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class BOUNCE_BACK_2_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

    UFUNCTION (BlueprintCallable, Category = Checkpoint)
    void GetSpawnPoint (FVector & loc, FRotator & rot);

private:
    UPROPERTY (VisibleAnywhere, BluePrintReadOnly, Category = Checkpoint, meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent *Mesh;

    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Checkpoint, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent *DetectionArea;

    TArray <FVector> SpawnOffsets;
};
