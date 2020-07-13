// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bounce_Back_2Ball.h"
#include "Door.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//state of door variables
	Opening = Closing = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MY BOX COMPONENT"));
	BoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	RootComponent = BoxComponent;

	Archway = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY ARCHWAY"));
	Archway->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	Archway->SetupAttachment(RootComponent);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY DOOR"));
	Door->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	Door->SetupAttachment(RootComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	//if door swings open
	if (SwingOpen)
	{
		//set direction of rotation
		if (clockwiseRotation)
		{
			//clockwise
			RotateValue = 90.0f;
		}
		else
		{
			//anti-clockwise
			RotateValue = -90.0f;
		}
	}
	//door raises open
	else
	{
		//set intial placement of the door mesh
		InitialLocation = Door->RelativeLocation;

		//get local bounds
		FVector min,max;
		Door->GetLocalBounds(min, max);

		//get scale of mesh
		FVector scale = max - min;

		//set final location of the door mesh, once raised
		FinalLocation = InitialLocation + FVector(0.0f, 0.0f, scale.Z);
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (SwingOpen)
	{
		DoorRotation = Door->RelativeRotation;
	}
	else
	{
		DoorLocation = Door->RelativeLocation;
	}

	//if theres a switch assigned to the door
	if (Lock != nullptr)
	{
		if (Lock->ReadyToOpen())
		{
			Lock->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
			Opening = true;
		}
	}

	//if theres a switch assigned to the door
	if (Switch != nullptr)
	{
		//if there a override switch assigned to the door
		if (OverrideSwitch != nullptr)
		{
			//if override is activated
			if (OverrideSwitch->GetIsActivated())
			{
				//override door switch
				Switch->OverrideSwitch();
			}
		}
		
		//if pressed
		if (Switch->GetIsActivated())
		{
			//open door
			Opening = true;
			Closing = false;
		}
		//if not pressed
		else
		{
			//close door
			Opening = false;
			Closing = true;
		}
	}

	//if door is opening
	if (Opening)
	{
		if (SwingOpen)
		{
			//rotatational lerp open
			Door->SetRelativeRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(RotateValue, 0.0f, 0.0f)), 0.01f));
		}
		else
		{
			//positional lerp open
			Door->SetRelativeLocation(FMath::Lerp(DoorLocation, FinalLocation, 0.01f));
		}
		
	}
	//if door is closing
	else if (Closing)
	{
		if (SwingOpen) 
		{
			//rotational lerp close
			Door->SetRelativeRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(0.0f, 0.0f, 0.0f)), 0.01f));
		}
		else
		{
			//positional lerp close
			Door->SetRelativeLocation(FMath::Lerp(DoorLocation, InitialLocation, 0.01f));
		}
	}
}

void ADoor::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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
				//if there is lock assigned
				if (Lock != nullptr)
				{
					if(Ball->GetHasKey())
						Lock->OpenLock();
				}
			}
		}
	}
}

void ADoor::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		
	}
}

