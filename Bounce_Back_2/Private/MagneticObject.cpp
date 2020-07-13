// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "MagneticObject.h"
#include "Magna.h"
#include "Lanos.h"
#include "Spike.h"
#include "Marble.h"
#include "TriggerButton.h"

// Sets default values
AMagneticObject::AMagneticObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create mesh component
	ObjModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	RootComponent = ObjModel;
	MagneticZone = CreateDefaultSubobject <UBoxComponent>(TEXT("MagnetBox"));
	MagneticZone->SetRelativeLocationAndRotation(FVector(-3.5f, -6.2f, 222.0f), FRotator(0.0f, 0.0f, 0.0f));
	MagneticZone->SetBoxExtent(FVector(6.0f, 6.0f, 13.5f), true);
	
	MagneticZone->SetupAttachment(RootComponent);
	KillZone = CreateDefaultSubobject <UBoxComponent>(TEXT("KillBox"));;
	KillZone->SetRelativeLocationAndRotation(FVector(-3.5f, -6.2f, 222.0f), FRotator(0.0f, 0.0f, 0.0f));
	KillZone->SetBoxExtent(FVector(6.0f, 6.0f, 13.5f), true);
	
	KillZone->SetupAttachment(RootComponent);
	TriggerButton = nullptr;
}

// Called when the game starts or when spawned
void AMagneticObject::BeginPlay()
{
	Super::BeginPlay();
	//ObjModel->SetSimulatePhysics(false);#

	if (objType != MagnetType::VE_ATTRACT)
		KillZone->Deactivate();
	MagneticZone->OnComponentBeginOverlap.AddDynamic(this, &AMagneticObject::MagneticBeginOverlap);
	MagneticZone->OnComponentEndOverlap.AddDynamic(this, &AMagneticObject::MagneticEndOverlap);
	KillZone->OnComponentBeginOverlap.AddDynamic(this, &AMagneticObject::KillZoneBeginOverlap);
	MagneticZone->SetGenerateOverlapEvents(true);
	UE_LOG(LogTemp, Warning, TEXT("Mag obj begin play func"));


}

// Called every frame
void AMagneticObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TriggerButton)
		if(TriggerButton->bIsPressed)
			bActive = false;
}


void AMagneticObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//Super::NotifyActorBeginOverlap(OtherActor);

	//AMagna* pMagna = Cast<AMagna>(OtherActor);

	////if magna is overlapping
	//if (pMagna)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("#Magna Overlap"));
	//	pMagna->addMagneticZone(this);
	//	ObjModel->SetSimulatePhysics(true);
	//	if (objType == MagnetType::VE_MOVEABLE)
	//	{
	//		UPrimitiveComponent* pComp = Cast<UPrimitiveComponent>(ObjModel);
	//		pComp->bRenderCustomDepth = true; //turn on outline
	//		pComp->MarkRenderStateDirty();
	//	}
	//}
}

void AMagneticObject::NotifyActorEndOverlap(AActor* OtherActor)
{
	//Super::NotifyActorEndOverlap(OtherActor);

	//AMagna* pMagna = Cast<AMagna>(OtherActor);

	////if magna is overlapping
	//if (pMagna)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("#Magna Overlap"));
	//	pMagna->removeMagneticZone(this);
	//	ObjModel->SetSimulatePhysics(false);
	//	if (objType == MagnetType::VE_MOVEABLE)
	//	{
	//		UPrimitiveComponent* pComp = Cast<UPrimitiveComponent>(ObjModel);
	//		pComp->bRenderCustomDepth = false; //turn outline off
	//		pComp->MarkRenderStateDirty();
	//	}
	//}
}

void AMagneticObject::MagneticBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//Super::NotifyActorBeginOverlap(otherActor);

	AMagna* pMagna = Cast<AMagna>(otherActor);
	//UE_LOG(LogTemp, Warning, TEXT("something overlaps"));
	//if magna is overlapping
	if (pMagna)
	{
		UE_LOG(LogTemp, Warning, TEXT("#Magna Overlap"));
		pMagna->addMagneticZone(this);
		//ObjModel->SetSimulatePhysics(true);
		if (objType == MagnetType::VE_MOVEABLE || objType == MagnetType::VE_FORCE_MOVEABLE)
		{
			UPrimitiveComponent* pComp = Cast<UPrimitiveComponent>(ObjModel);
			pComp->bRenderCustomDepth = true; //turn on outline
			pComp->MarkRenderStateDirty();
		}
	}
}

void AMagneticObject::MagneticEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Super::NotifyActorEndOverlap(OtherActor);

	AMagna* pMagna = Cast<AMagna>(OtherActor);

	//if magna is overlapping
	if (pMagna)
	{
		UE_LOG(LogTemp, Warning, TEXT("#Magna endoverlap"));
		pMagna->removeMagneticZone(this);
		//ObjModel->SetSimulatePhysics(false);
		if (objType == MagnetType::VE_MOVEABLE || objType == MagnetType::VE_FORCE_MOVEABLE)
		{
			UPrimitiveComponent* pComp = Cast<UPrimitiveComponent>(ObjModel);
			pComp->bRenderCustomDepth = false; //turn outline off
			pComp->MarkRenderStateDirty();
		}
	}
}

void AMagneticObject::KillZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	ALanos* pLanos = Cast<ALanos>(otherActor);
	AMagna* pMagna = Cast<AMagna>(otherActor);
	ASpike* pSpike = Cast<ASpike>(otherActor);
	AMarble* pMarb = Cast<AMarble>(otherActor);
	if (objType == MagnetType::VE_FORCE_MOVEABLE)
	{
		if (pLanos)
		{
			UE_LOG(LogTemp, Warning, TEXT(" ded"));
			pLanos->Kill(false);
		}
		if (pSpike)
		{
			UE_LOG(LogTemp, Warning, TEXT(" ded"));
			pSpike->Kill(false);
		}
		if (pMarb)
		{
			UE_LOG(LogTemp, Warning, TEXT(" ded"));
			pMarb->Kill(false);
		}
	}

	if (objType == MagnetType::VE_ATTRACT && bActive)
	{
		if (pMagna)
			pMagna->Kill(false);
	}
}