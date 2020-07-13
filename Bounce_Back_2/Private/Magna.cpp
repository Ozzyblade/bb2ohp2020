// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Magna.h"
#include "Bounce_Back_2GameMode.h"
#include "Hazards/Pencil.h"
#include "Hazards/Pen.h"
#include "Hazards/Water.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/World.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "GameFramework/PlayerController.h"

AMagna::AMagna()
{
	//create mesh component
	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	Model->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Model->SetRelativeScale3D(FVector(0.85f, 0.85f, 0.85f));
	Model->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Ball->SetPhysicsMaxAngularVelocity(1500.0f);
	Ball->SetRenderInMainPass(false);

	Model->SetupAttachment(RootComponent);

	bPulling = false;
	bPushing = false;
}

void AMagna::BeginPlay()
{
	Super::BeginPlay();

	gameMode->Magna = this;
}

void AMagna::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	for(AMagneticObject* pMag : magneticZonesActive)
	{
		if (pMag->bActive && (pMag->objType != MagnetType::VE_MOVEABLE && pMag->objType != MagnetType::VE_FORCE_MOVEABLE))
		{
			FVector myLocation = GetActorLocation();
			FVector loc = pMag->KillZone->GetComponentLocation();
			float distance = FVector::Dist2D(loc, myLocation);
			distance = abs(distance);
			UE_LOG(LogTemp, Warning, TEXT("poop %s"), *loc.ToString());
			UE_LOG(LogTemp, Warning, TEXT("location2 %s"), *pMag->GetActorLocation().ToString());
			float intensity = 1 / pow(distance, 2); //inverse square law
			intensity *= 10000000; //too weak keeping it low
			FVector dir = loc - myLocation;
			dir.Normalize();
			if (pMag->objType == MagnetType::VE_REPULSE)
			{
				dir = -dir;
				UE_LOG(LogTemp, Warning, TEXT("repulse"));
			}
			//UE_LOG(LogTemp, Warning, TEXT(" intensitit %f"), intensity);
			FVector finalForce;

			finalForce = dir * intensity  * pMag->magneticStrength * DeltaSeconds;
			UE_LOG(LogTemp, Warning, TEXT(" intensitit %f"), intensity);
			//UE_LOG(LogTemp, Warning, TEXT(" intensitit %f"), intensity);
			UE_LOG(LogTemp, Warning, TEXT("FORCE ADDED %s"), *finalForce.ToString());
			//finalForce.Z = 0;
			Ball->AddImpulse(finalForce);
			//UE_LOG(LogTemp, Warning, TEXT("ADDING FORCE"));



		}
		if ((pMag->objType == MagnetType::VE_MOVEABLE || pMag->objType == MagnetType::VE_FORCE_MOVEABLE) && (bPushing || bPulling)) //in range of a moveable object we can influence
		{
			FVector myLocation = GetActorLocation();
			FVector dir = pMag->GetActorLocation() - myLocation;
			dir.Normalize();
			if (bPulling)
				dir = -dir;

			FVector force;

			force = dir * magneticPower * DeltaSeconds;
			UE_LOG(LogTemp, Warning, TEXT("FORCE ADDED %s"), *force.ToString());
			force.Z = 0;
			FVector newlocation = pMag->GetActorLocation() + force;
			if(pMag->objType == MagnetType::VE_MOVEABLE)
				pMag->SetActorLocation(newlocation);
			else
			{
				
				force = dir * (magneticPower);
				pMag->ObjModel->SetSimulatePhysics(true);
				UE_LOG(LogTemp, Warning, TEXT("FORCE ADDED %s"), *force.ToString());
				pMag->ObjModel->AddImpulse(force);
			}

			/*UPrimitiveComponent* pComp = Cast<UPrimitiveComponent>(pMag->ObjModel);
			pComp->bRenderCustomDepth = true;
			pComp->MarkRenderStateDirty();*/
			/*UStaticMeshComponent* pMesh = Cast<UStaticMeshComponent>(pMag->GetRootComponent());
			pMesh->AddForce(force);*/
		}
	}
}

void AMagna::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMagna::pushAction);
	PlayerInputComponent->BindAction("Action2", IE_Pressed, this, &AMagna::pullAction);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &AMagna::stoppushAction);
	PlayerInputComponent->BindAction("Action2", IE_Released, this, &AMagna::stopPullAction);
}

void AMagna::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

void AMagna::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
}

void AMagna::UnPossessed()
{
	Super::UnPossessed();
}

void AMagna::PossessedBy(class AController *NewController)
{
	Super::PossessedBy(NewController);

	if (!NewController->IsPlayerController()) return;
}

void AMagna::addMagneticZone(AMagneticObject* magObj)
{
	magneticZonesActive.Emplace(magObj);
	//UE_LOG(LogTemp, Warning, TEXT("#Magna Overlap"));
}

void AMagna::removeMagneticZone(AMagneticObject* magObj)
{
	magneticZonesActive.Remove(magObj);
	if (bPushing)
		bPushing = false;
	//UE_LOG(LogTemp, Warning, TEXT("#Magna end overlap"));
}

