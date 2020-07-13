// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "FallingObject.h"


// Sets default values
AFallingObject::AFallingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_pModel->SetSimulatePhysics(true);
	m_pModel->SetEnableGravity(true);
	RootComponent = m_pModel;
}

// Called when the game starts or when spawned
void AFallingObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFallingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AFallingObject::fallOver()
{
	m_pModel->AddForce(FVector(m_ForceX, m_ForceY, m_ForceZ), NAME_None, true);

	setFallState(true);
}

bool AFallingObject::getFallState()
{
	return m_fallen;
}

void AFallingObject::setFallState(bool s)
{
	m_fallen = s;
}