// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Chase.h"
#include "Bounce_Back_2Ball.h"
#include "DarkZone.h"

// Sets default values
AChase::AChase() :
	m_pChasedCharacter(nullptr),
	m_progress(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* pRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = pRoot;

	//set up trigger volumes for start and end of chase
	StartTriggerVolume = CreateDefaultSubobject<UBoxComponent>("StartVolume");
	StartTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AChase::OnOverlapStart);

	EndTriggerVolume = CreateDefaultSubobject<UBoxComponent>("EndVolume");
	EndTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AChase::OnOverlapEnd);

	//UPointLightComponent* pLight = CreateDefaultSubobject<UPointLightComponent>("Light1");
	//pLight->SetWorldLocation({ 1357.182983, 3797.769287, 5548.888184 });
	//Lights.Push(pLight);
	//
	//pLight = CreateDefaultSubobject<UPointLightComponent>("Light2");
	//pLight->SetWorldLocation({ 371.538391, 3559.344238, 5758.924805 });
	//Lights.Push(pLight);
	//
	//pLight = CreateDefaultSubobject<UPointLightComponent>("Light3");
	//pLight->SetWorldLocation({ -573.959595, 3559.344238, 5758.924316 });
	//Lights.Push(pLight);

}

// Called when the game starts or when spawned
void AChase::BeginPlay()
{
	Super::BeginPlay();
	
	for (auto iter : DarkZones)
	{
		m_darkZoneStartingScales.Push(iter->GetActorScale3D());
		m_darkZoneStartingPositions.Push(iter->GetActorLocation());
	}

	m_remainingTimeForStep = DarkZoneTimings[0];
}

// Called every frame
void AChase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if there is a chase in progress
	if (m_pChasedCharacter != nullptr)
	{
		if (m_progress < DarkZoneTimings.Num())
		{
			m_remainingTimeForStep -= DeltaTime;
			if (m_remainingTimeForStep > 0.0f)
			{
				float lerpModifier = (DarkZoneTimings[m_progress] - m_remainingTimeForStep) / DarkZoneTimings[m_progress];

				DarkZones[m_progress]->SetActorScale3D(FMath::Lerp(m_darkZoneStartingScales[m_progress],
																   DarkZoneTargetScales[m_progress],
																   lerpModifier));
				
				DarkZones[m_progress]->SetActorLocation(FMath::Lerp(m_darkZoneStartingPositions[m_progress],
																    DarkZoneTargetPositions[m_progress],
																    lerpModifier));
			}
			else
			{
				if (++m_progress < DarkZoneTimings.Num())
				{
					m_remainingTimeForStep = DarkZoneTimings[m_progress];		
				}
			}
		}
	}
}

void AChase::reset()
{
	m_pChasedCharacter = nullptr;
	m_progress = 0;
	for (int i = 0; i < DarkZones.Num(); ++i)
	{
		DarkZones[i]->SetActorScale3D(m_darkZoneStartingScales[i]);
		DarkZones[i]->SetActorLocation(m_darkZoneStartingPositions[i]);
	}
	m_remainingTimeForStep = DarkZoneTimings[0];
}

void AChase::OnOverlapStart(UPrimitiveComponent* pOverlappedComponent, AActor* pOtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if there is NO chase in progress
	if (m_pChasedCharacter == nullptr)
	{
		ABounce_Back_2Ball* pCharacter = Cast<ABounce_Back_2Ball>(pOtherActor);
		if (pCharacter != nullptr)
		{
			//start chase
			m_pChasedCharacter = pCharacter;
		}
	}

	//for (int i = 0; i < 3; ++i) {
	//	Lights[i]->SetIntensity(0.f);
	//}

	//for (auto iter : Lights)
	//{
	//	iter->SetWorldLocation({ 900000.f, 0.f, 0.f });
	//}
}

void AChase::OnOverlapEnd(UPrimitiveComponent* pOverlappedComponent, AActor* pOtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if there is a chase in progress
	if (m_pChasedCharacter != nullptr)
	{
		ABounce_Back_2Ball* pCharacter = Cast<ABounce_Back_2Ball>(pOtherActor);
		if (m_pChasedCharacter == pCharacter)
		{
			//end chase
			m_pChasedCharacter = nullptr;
		}
	}
}