// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bounce_Back_2Ball.h"
#include "Lock.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	DummyRoot->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	DummyRoot->RelativeRotation = FRotator(0.f, 180.f, 0.f);
	RootComponent = DummyRoot;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(TEXT("/Game/Assets/BB8_Assets/BB8_Meshes/BB2_Lock_Body_Asset.BB2_Lock_Body_Asset")); check(BodyMesh.Succeeded());
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PoleMesh(TEXT("/Game/Assets/BB8_Assets/BB8_Meshes/BB2_Lock_Poles_Asset.BB2_Lock_Poles_Asset")); check(PoleMesh.Succeeded());

	Lock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY LOCK BODY"));
	Lock->SetStaticMesh(BodyMesh.Object);
	Lock->SetupAttachment(RootComponent);

	Poles = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY LOCK POLES"));
	Poles->SetStaticMesh(PoleMesh.Object);
	Poles->SetRelativeScale3D(FVector(1.f, 1.2, 1.f));
	Poles->SetupAttachment(RootComponent);

	readyToOpen = false;
	locked = true;
	timer = 2.f;
}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (locked == false)
	{
		timer -= DeltaTime;
		if (timer > 0) {

			Poles->SetWorldScale3D(UKismetMathLibrary::VInterpTo(Poles->GetRelativeTransform().GetScale3D(), FVector(1.f, 0.8, 1.f), DeltaTime, 100.f));
		}
	}

	if (timer <= 0)
	{
		readyToOpen = true;
	}
}

void ALock::OpenLock()
{
	locked = false;
}

bool ALock::ReadyToOpen()
{
	return readyToOpen;
}
