// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "GlowSticker.h"
#include "Lanos.h"

int AGlowSticker::numOfStickers = 0;

// Sets default values
AGlowSticker::AGlowSticker() :
	m_pParentActor(nullptr),
	m_totalStickTime(10.0f),
	m_stickTimeRemaining(m_totalStickTime),
	m_canPickup(true),
	m_waitingToDetach(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = m_pMesh;

	m_pLight = CreateDefaultSubobject<UPointLightComponent>("Light");

	OnActorBeginOverlap.AddDynamic(this, &AGlowSticker::OnOverlap);
}

// Called when the game starts or when spawned
void AGlowSticker::BeginPlay()
{
	Super::BeginPlay();
	m_defaultLocation = GetActorLocation();
}

// Called every frame
void AGlowSticker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//when sticking on the player
	if (m_pParentActor != nullptr && !m_waitingToDetach)
	{
		m_stickTimeRemaining -= DeltaTime;
		if (m_stickTimeRemaining < 0.0f)
		{
			m_waitingToDetach = true;
			m_stickTimeRemaining = m_totalStickTime;
		}
	}

	//when sticking to player but waiting for ideal situation to unstick
	if (m_waitingToDetach)
	{
		//ensures that the sticker cannot detach when below the character
		if (GetActorLocation().Z > m_pParentActor->GetActorLocation().Z)
		{
			//if sticker is no longer sticky, detach from the player and reset the timer
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			numOfStickers--;
			if (numOfStickers == 0)
			{
				ALanos* tempLanos = Cast<ALanos>(m_pParentActor);
				tempLanos->SetIsGlowing(false);
			}
			m_pParentActor = nullptr;
			m_waitingToDetach = false;

			//enable simulation of physics so the sticker comes to rest
			m_pMesh->SetSimulatePhysics(true);
			m_pMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
		}
	}

	//when not sticking on the player but hasn't come to a rest yet
	if (m_pParentActor == nullptr && !m_canPickup)
	{
		if (m_pMesh->GetPhysicsLinearVelocity().Size() < 0.01f)
		{
			//can stick to player again
			m_canPickup = true;

			//disable physics as the sticker has basically come to rest
			m_pMesh->SetSimulatePhysics(false);
			m_pMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		}
	}
}

void AGlowSticker::reset()
{
	//if attached to Lanos, detach and reset relevant parameters
	if (m_pParentActor != nullptr) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		m_pParentActor = nullptr;
		m_waitingToDetach = false;
		m_stickTimeRemaining = m_totalStickTime;
		m_canPickup = true;
	}

	//set position and rotation back to where the sticker started
	SetActorLocationAndRotation(m_defaultLocation, { 0.f, 0.f, 0.f });

	//remove existing momentum applied to sticker
	m_pMesh->SetAllPhysicsLinearVelocity({ 0.f, 0.f, 0.f });
}

void AGlowSticker::OnOverlap(AActor* pOverlappedActor, AActor* pOtherActor)
{
	ALanos* tempLanos = Cast<ALanos>(pOtherActor);
	if (m_canPickup && tempLanos != nullptr)
	{
		AttachToActor(pOtherActor, FAttachmentTransformRules::KeepWorldTransform);
		numOfStickers++;
		if (tempLanos->IsCurrentlyGlowing())
		{
			tempLanos->SetIsGlowing(true);
		}
		m_pParentActor = pOtherActor;
		m_canPickup = false;
	}
}