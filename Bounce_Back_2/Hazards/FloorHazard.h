// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorHazard.generated.h"

UCLASS()
class BOUNCE_BACK_2_API AFloorHazard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorHazard();

	float GetDampening();

	class UPhysicalMaterial* GetMaterial();

protected:

	UPROPERTY(VisibleAnywhere, Category = Floor)
	class UDecalComponent* decal;

	UPROPERTY(VisibleAnywhere, Category = Floor)
	class UBoxComponent* CollisionBox;

	//UPROPERTY(VisibleAnywhere, Category = Floor)
	//class UPhysicsMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPhysicalMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speedDampening;

public:	


};
