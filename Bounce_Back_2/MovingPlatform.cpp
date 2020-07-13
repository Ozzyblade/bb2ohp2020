// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bounce_Back_2Ball.h"
#include "MovingPlatform.h"


// Sets default values
AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	RootComponent = DummyRoot;

	MovingRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MovingRoot"));
	MovingRoot->SetupAttachment(RootComponent);

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY PLATFORM"));
	Platform->SetRelativeLocation(FVector(0.0f, 10.0f, -50.0f));
	Platform->SetupAttachment(MovingRoot);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MY BOX COMPONENT"));
	BoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	BoxComponent->SetupAttachment(Platform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StartMesh(TEXT("/Game/Assets/BB8_Assets/BB8_Meshes/StartPoint.StartPoint")); check(StartMesh.Succeeded());
	StartPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY START POINT"));
	StartPoint->SetStaticMesh(StartMesh.Object);

	StartPoint->SetupAttachment(RootComponent);
	StartPoint->SetRelativeLocation(FVector(0, 20, -100));
	StartPoint->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StartPoint->SetHiddenInGame(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> EndMesh(TEXT("/Game/Assets/BB8_Assets/BB8_Meshes/EndPoint.EndPoint")); check(EndMesh.Succeeded());
	EndPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY END POINT"));
	EndPoint->SetStaticMesh(EndMesh.Object);

	EndPoint->SetupAttachment(RootComponent);
	EndPoint->SetRelativeLocation(FVector(0, 20, 0.0));
	EndPoint->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EndPoint->SetHiddenInGame(true);


	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatform::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMovingPlatform::OnOverlapEnd);

	ifActivated = false;
	canMove = true;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	//if moving only vertically, make start and end point line up horizontally
	if (VerticalMovement)
	{
		EndPoint->SetRelativeLocation(FVector(StartPoint->RelativeLocation.X, EndPoint->RelativeLocation.Y, EndPoint->RelativeLocation.Z));
	}
	//if moving only horizontally, make start and end point line up horizontally
	else
	{
		EndPoint->SetRelativeLocation(FVector(EndPoint->RelativeLocation.X, EndPoint->RelativeLocation.Y, StartPoint->RelativeLocation.Z));
	}

	if (continuous && needsActivating == false)
	{
		Moving = true;
	}
	else
	{
		Moving = false;
	}

	if (needsActivating)
	{
		canMove = false;
	}
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StartSwitch != nullptr)
	{
		if (StartSwitch->GetIsActivated() && needsActivating == false)
		{
			atStart = false;
			Moving = true;
		}
		else if (StartSwitch->GetIsActivated() && needsActivating == true)
		{
			if (ifActivated == false)
			{
				Moving = true;
				ifActivated = true;
			}
		}
	}

	if (EndSwitch != nullptr)
	{
		if (EndSwitch->GetIsActivated() && needsActivating == false)
		{
			atStart = true;
			Moving = true;
		}
		else if (EndSwitch->GetIsActivated() && needsActivating == true)
		{
			if (ifActivated == false)
			{
				Moving = true;
				ifActivated = true;
			}
		}
	}

	if (Moving)
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;

		if (atStart)
		{
			MoveToEnd(LatentInfo);
		}
		else
		{
			MoveToStart(LatentInfo);
		}
	}
}

void AMovingPlatform::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// check for player
		{
			// cast actor to player
			ABounce_Back_2Ball* const Ball = Cast <ABounce_Back_2Ball>(OtherActor);

			// if cast is successful
			if (Ball)
			{
				if (canMove)
				{
					Moving = true;
					canMove = false;
				}
			}
		}
	}
}

void AMovingPlatform::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{

	}
}

void AMovingPlatform::TimerEnd()
{
	canMove = true;
}

void AMovingPlatform::TimerEndContinuous()
{
	Moving = true;
}

void AMovingPlatform::MoveToStart(FLatentActionInfo LatentInfo)
{
	//set to move toward start point
	UKismetSystemLibrary::MoveComponentTo(MovingRoot, StartPoint->RelativeLocation, FRotator(0.0f, 0.0f, 0.0f), false, false, movementDuration, false, EMoveComponentAction::Type::Move, LatentInfo);

	if (continuous)
	{
		float TimerDuration = movementDuration + waitDuration;
		//set a timer to set whether lerp is over
		//allows the platform to move automatically
		GetWorld()->GetTimerManager().SetTimer(moveTimer, this, &AMovingPlatform::TimerEndContinuous, TimerDuration);
	}
	else
	{
		//set a timer to set whether lerp is over
		//allows the platform to move if player collides with
		GetWorld()->GetTimerManager().SetTimer(moveTimer, this, &AMovingPlatform::TimerEnd, movementDuration);
	}

	Moving = false;
	atStart = true;
}

void AMovingPlatform::MoveToEnd(FLatentActionInfo LatentInfo)
{
	//set to move toward end point
	UKismetSystemLibrary::MoveComponentTo(MovingRoot, EndPoint->RelativeLocation, FRotator(0.0f, 0.0f, 0.0f), false, false, movementDuration, false, EMoveComponentAction::Type::Move, LatentInfo);

	if (continuous)
	{
		float TimerDuration = movementDuration + waitDuration;
		//set a timer to set whether lerp is over
		//allows the platform to move automatically
		GetWorld()->GetTimerManager().SetTimer(moveTimer, this, &AMovingPlatform::TimerEndContinuous, TimerDuration);
	}
	else
	{
		//set a timer to set whether lerp is over
		//allows the platform to move if player collides with
		GetWorld()->GetTimerManager().SetTimer(moveTimer, this, &AMovingPlatform::TimerEnd, movementDuration);
	}

	Moving = false;
	atStart = false;
}

