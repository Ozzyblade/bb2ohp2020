// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Spike.h"
#include "BallBridge.h"


// Sets default values
ABallBridge::ABallBridge()
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


}

// Called when the game starts or when spawned
void ABallBridge::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABallBridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABallBridge::fall()
{
	m_pModel->SetSimulatePhysics(true);
	m_pModel->SetEnableGravity(true);
	RootComponent = m_pModel;
	m_pModel->AddForce(FVector(m_ForceX, m_ForceY, m_ForceZ), NAME_None, true);
}

bool ABallBridge::getFallen()
{
	return m_Fallen;
}

void ABallBridge::setFallen(bool state)
{
	m_Fallen = state;
}

void ABallBridge::NotifyActorBeginOverlap(AActor* pOtherActor)
{
	ABounce_Back_2Ball* tempBall = Cast<ABounce_Back_2Ball>(pOtherActor);

	if (!m_Fallen && tempBall != nullptr)
	{
		m_pModel->SetSimulatePhysics(true);
		m_pModel->SetEnableGravity(true);
		m_Fallen = true;
		m_pModel->SetCollisionProfileName(TEXT("PhysicsActor"));
		RootComponent = m_pModel;
		m_pModel->AddForce(FVector(m_ForceX, m_ForceY, m_ForceZ), NAME_None, true);
	}
}

