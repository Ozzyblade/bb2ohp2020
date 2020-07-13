// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pencil.generated.h"

UCLASS()
class BOUNCE_BACK_2_API AStationery_Pencil : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    AStationery_Pencil();

	bool isBlunted();

	void blunted();

protected:
    UPROPERTY (VisibleAnywhere, Category = "Pencil")
    class UStaticMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Pencil")
		class UStaticMeshComponent *m_pBrokenMesh;

    UPROPERTY (VisibleAnywhere, Category = "Pencil")
    class UBoxComponent *CollisionBox;


private:

	bool m_blunt;
};
