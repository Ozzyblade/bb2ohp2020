// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Spike.h"
#include "SpikeBridge.h"


// Sets default values
ASpikeBridge::ASpikeBridge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pFallTrigger = CreateDefaultSubobject<UBoxComponent>("Fall Trigger");
	m_pFallTrigger->SetupAttachment(RootComponent);

	m_pModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_pModel->SetSimulatePhysics(false);
	m_pModel->SetEnableGravity(false);
	RootComponent = m_pModel;
	m_Fallen = false;
	m_ActualMass = 1000.0f;
}

// Called when the game starts or when spawned
void ASpikeBridge::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpikeBridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpikeBridge::fall()
{
	m_pModel->SetSimulatePhysics(true);
	m_pModel->SetEnableGravity(true);
	RootComponent = m_pModel;
	m_pModel->AddForce(FVector(m_ForceX, m_ForceY, m_ForceZ), NAME_None, true);
}

bool ASpikeBridge::getFallen()
{
	return m_Fallen;
}

void ASpikeBridge::setFallen(bool state)
{
	m_Fallen = state;
}

void ASpikeBridge::NotifyActorBeginOverlap(AActor* pOtherActor)
{
	ASpike* tempSpike = Cast<ASpike>(pOtherActor);

	if (!m_Fallen && tempSpike != nullptr)
	{
		tempSpike->SpikeAction();
		m_pModel->SetSimulatePhysics(true);
		m_pModel->SetEnableGravity(true);
		m_Fallen = true;
		m_pModel->SetCollisionProfileName(TEXT("PhysicsActor"));
		RootComponent = m_pModel;
		m_pModel->AddForce(FVector(m_ForceX, m_ForceY, m_ForceZ), NAME_None, true);
	}
}