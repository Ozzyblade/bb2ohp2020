// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Water.generated.h"

/**
 * 
 */
UCLASS()
class BOUNCE_BACK_2_API AWater : public AActor
{
    GENERATED_BODY ()

public:
    AWater ();
	
	float getDampening() { return speedDampening; }
protected:
    UPROPERTY (VisibleAnywhere, Category = Water)
    class UDecalComponent *decal;
    
    UPROPERTY (VisibleAnywhere, Category = Water)
    class UBoxComponent *CollisionBox1;
    UPROPERTY (VisibleAnywhere, Category = Water)
    class UBoxComponent *CollisionBox2;
    UPROPERTY (VisibleAnywhere, Category = Water)
    class UBoxComponent *CollisionBox3;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
	float speedDampening;
};
