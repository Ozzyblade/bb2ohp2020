// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pen.generated.h"

UCLASS()
class BOUNCE_BACK_2_API APen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APen();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Pen")
		class UStaticMeshComponent *m_pMesh;

	UPROPERTY(VisibleAnywhere, Category = "Pen")
		class UBoxComponent *m_pCollisionBox;

	
};
