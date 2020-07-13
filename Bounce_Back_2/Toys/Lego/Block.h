// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Block.generated.h"

UCLASS()
class BOUNCE_BACK_2_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock ();

    // Called every frame
    virtual void Tick (float DeltaSeconds) override;

    void enable ();
    void disable ();

    bool GetIsAboutToDie () const;
    void Kill ();

public:
    // how long to stay alive after being touched by Spike
    UPROPERTY (EditAnywhere, Category = "lego block")
    float KillTime;
    // higher -> flash more often on death
    UPROPERTY (EditAnywhere, Category = "lego block",  meta = (ClampMin = "0.1", UIMin = "0.1"))
    float FlashFrequency;
    // higher -> flash on for longer
    UPROPERTY (EditAnywhere, Category = "lego block", meta = (ClampMin = "-0.9", ClampMax = "0.9", UIMin = "-0.9", UIMax = "0.9"))
    float FlashOffset;

protected:
    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "lego block")
    class UStaticMeshComponent *Mesh;

    UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "lego block")
    float ActualMass;

private:
    bool IsAboutToDie;
    float TimeSinceKill;
    bool IsDead;
};
