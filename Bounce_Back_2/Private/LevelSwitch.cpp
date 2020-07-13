// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "LevelSwitch.h"
#include "Bounce_Back_2Ball.h"

ALevelSwitch::ALevelSwitch()
{

}

void ALevelSwitch::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ALevelSwitch::OnOverlapBegin);
}

void ALevelSwitch::OnOverlapBegin(class AActor* actor, class AActor* other)
{
	if (other && (other != this) && other == SpecificActor)
	{
		//printf("%s Entered Box", *other->GetName());
		{
			ABounce_Back_2Ball* ball = Cast<ABounce_Back_2Ball>(other);

			if (ball)
			{
				// Switch the current level
				FLatentActionInfo LatentInfo;
				UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
			}
		}
	}
}