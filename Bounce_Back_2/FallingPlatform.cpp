// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "FallingPlatform.h"


// Sets default values
AFallingPlatform::AFallingPlatform() :
	m_clipPlayed(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//set up volume used for triggers
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>("Volume");

	//set up mesh components
	TrianglesMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrianglesMesh");
	UpperPillarMesh = CreateDefaultSubobject<UStaticMeshComponent>("UpperPillarMesh");
	LowerPillarMesh = CreateDefaultSubobject<UStaticMeshComponent>("LowerPillarMesh");

	//set up audio clip
	AudioClip = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioClip->bAutoActivate = false;

	OnActorBeginOverlap.AddDynamic(this, &AFallingPlatform::OnOverlap);
}

// Called when the game starts or when spawned
void AFallingPlatform::BeginPlay()
{
	Super::BeginPlay();
	m_defaultLocation = GetActorLocation();
	m_defaultRotation = GetActorRotation().Quaternion();
}

// Called every frame
void AFallingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFallingPlatform::reset()
{
	//stop any timers to prevent a collapse after respawn
	GetWorldTimerManager().ClearTimer(m_collapseTimer);

	//reset relevant parameters
	m_clipPlayed = false;
	AudioClip->Stop();

	//set position and rotation back to where the platform started
	TriggerVolume->SetWorldLocation(m_defaultLocation);
	TriggerVolume->SetWorldRotation(m_defaultRotation);
	TrianglesMesh->SetWorldLocation(m_defaultLocation + FVector(0.f, 0.f, -30.f));
	TrianglesMesh->SetWorldRotation(m_defaultRotation);
	UpperPillarMesh->SetWorldLocation(m_defaultLocation + FVector(0.f, 0.f, -30.f));
	UpperPillarMesh->SetWorldRotation(m_defaultRotation);
	LowerPillarMesh->SetWorldLocation(m_defaultLocation + FVector(0.f, 0.f, -30.f));
	LowerPillarMesh->SetWorldRotation(m_defaultRotation);

	//disable physics simulation
	TrianglesMesh->SetSimulatePhysics(false);
	UpperPillarMesh->SetSimulatePhysics(false);
	LowerPillarMesh->SetSimulatePhysics(false);

	//remove existing momentum applied to platform pieces
	TrianglesMesh->SetAllPhysicsLinearVelocity({ 0.f, 0.f, 0.f });
	UpperPillarMesh->SetAllPhysicsLinearVelocity({ 0.f, 0.f, 0.f });
	LowerPillarMesh->SetAllPhysicsLinearVelocity({ 0.f, 0.f, 0.f });
}

void AFallingPlatform::OnOverlap(AActor* pOverlappedActor, AActor* pOtherActor)
{
	if (Collapses)
	{
		if (Cast<ABounce_Back_2Ball>(pOtherActor) != nullptr)
		{
			GetWorldTimerManager().SetTimer(m_collapseTimer, this, &AFallingPlatform::OnElapsed, CollapseTime, false);

			//if not played before, play the audio clip
			if (!m_clipPlayed) {
				m_clipPlayed = true;
				AudioClip->Play();
			}
		}
	}
}

void AFallingPlatform::OnElapsed()
{
	TrianglesMesh->SetSimulatePhysics(true);
	UpperPillarMesh->SetSimulatePhysics(true);
	LowerPillarMesh->SetSimulatePhysics(true);
}