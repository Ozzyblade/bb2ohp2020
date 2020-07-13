// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "DarkZone.h"


// Sets default values
ADarkZone::ADarkZone() :
	m_isTimerActive(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//set up volume used for triggers
	m_pVolume = CreateDefaultSubobject<UBoxComponent>("Volume");
	SetActorHiddenInGame(false);
	m_pVolume->bHiddenInGame = false;
	RootComponent = m_pVolume;
}

// Called when the game starts or when spawned
void ADarkZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADarkZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADarkZone::NotifyActorBeginOverlap(AActor* pOtherActor)
{
	//store character reference if they enter the darkness
	ABounce_Back_2Ball* pCharacter = Cast<ABounce_Back_2Ball>(pOtherActor);
	if (pCharacter != nullptr)
	{
		m_containedCharacters.Push(pCharacter);

		if (!m_isTimerActive)
		{
			m_isTimerActive = true;
			GetWorldTimerManager().SetTimer(m_respawnTimer, this, &ADarkZone::OnElapsed, m_respawnTime, false);
		}
	}
}

void ADarkZone::NotifyActorEndOverlap(AActor* pOtherActor)
{
	//remove character reference if they leave the darkness
	ABounce_Back_2Ball* pCharacter = Cast<ABounce_Back_2Ball>(pOtherActor);
	if (pCharacter != nullptr)
	{
		m_containedCharacters.Remove(pCharacter);
	}

	//reset timer if no characters remain in the darkness
	if (m_containedCharacters.Num() == 0)
	{
		//disable the timer
		GetWorldTimerManager().ClearTimer(m_respawnTimer);

		//flag that timer is no longer in use
		m_isTimerActive = false;
	}
}

void ADarkZone::OnElapsed()
{
	//if there are any characters in the darkness
	if (m_containedCharacters.Num() > 0)
	{
		if (m_isDeadly)
		{
			//force respawn on any contained characters
			for (ABounce_Back_2Ball* pCharacter : m_containedCharacters)
			{
				if (pCharacter->IsCurrentlyGlowing() == false)
				{
					pCharacter->Kill(false);
				}
			}

			//empty container of references
			m_containedCharacters.Empty();
		}

		//flag that timer is no longer in use
		m_isTimerActive = false;
	}
}