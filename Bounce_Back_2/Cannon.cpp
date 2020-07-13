// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Lanos.h"
#include <EngineGlobals.h>
#include "Engine/Engine.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the two meshes
	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	RootComponent = CannonMesh;
	CannonMuzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMuzzle"));
	CannonMuzzle->SetupAttachment(RootComponent);
	CannonImpulse = -5000.f;
	CannonHoldTime = 2.f;
	bMakeInvisible = true;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	CannonMuzzle->SetVisibility(false);
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACannon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Lanos != nullptr) {
		if (OtherActor == Lanos)
		{
			UE_LOG(LogTemp, Warning, TEXT("Off"));
			//FVector Test = Cast<APawn>(CannonMuzzle)->GetActorLocation();

			FVector Test = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 210.f);
			Lanos->GetBall()->SetWorldLocation(Test);
			Lanos->GetBall()->SetMobility(EComponentMobility::Static);
			Lanos->SetActorHiddenInGame(false);
			GetWorldTimerManager().SetTimer(TimerHandle_ButtonTimeLength, this, &ACannon::FireLanos, CannonHoldTime, false);
		}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Canon"));
	}
}

void ACannon::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

}

void ACannon::FireLanos()
{
	if (Lanos != nullptr) {
		Lanos->SetActorHiddenInGame(false);
		Lanos->GetBall()->SetMobility(EComponentMobility::Movable);
		if (!Lanos->Is2D)
			Lanos->GetBall()->SetAllPhysicsLinearVelocity(FVector(GetActorForwardVector() * CannonImpulse));
		else
			Lanos->GetBall()->SetAllPhysicsLinearVelocity(FVector(GetActorForwardVector() * CannonImpulse));

	}
}


