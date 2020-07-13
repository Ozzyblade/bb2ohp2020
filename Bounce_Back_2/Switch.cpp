// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Bounce_Back_2Ball.h"
#include "Marble.h"
#include "Lanos.h"
#include "Spike.h"
#include "PhysicsEngine/BodySetup.h"
#include "Switch.h"

// Sets default values
ASwitch::ASwitch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MY BOX COMPONENT"));
	BoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	BoxComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	RootComponent = BoxComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMesh(TEXT("/Game/Assets/BB8_Assets/BB8_Meshes/BB2_InnieSwitch.BB2_InnieSwitch")); check(BaseMesh.Succeeded());
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY SWITCH BASE"));
	Base->SetStaticMesh(BaseMesh.Object);
	Base->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Base->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));

	Base->GetBodySetup()->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseComplexAsSimple;
	Base->SetupAttachment(RootComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASwitch::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ASwitch::OnOverlapEnd);

	static ConstructorHelpers::FObjectFinder<UMaterial> MatRed(TEXT("/Game/Assets/BB8_Assets/BB8_Materials/Bed_Level_Materials/BB2_Innie_Switch_RedMat.BB2_Innie_Switch_RedMat")); check(MatRed.Succeeded());
	static ConstructorHelpers::FObjectFinder<UMaterial> MatGreen(TEXT("/Game/Assets/BB8_Assets/BB8_Materials/Bed_Level_Materials/BB2_Innie_Switch_GrnMat.BB2_Innie_Switch_GrnMat")); check(MatGreen.Succeeded());

	greenMat = MatGreen.Object;
	redMat = MatRed.Object;

	//initialisation of a dynamic material instance
	DynMaterial = Base->CreateAndSetMaterialInstanceDynamicFromMaterial(0, redMat);
	Base->SetMaterial(0, redMat);

	canBePressed = true;
	//state of the switch
	Overridden = Activated = false;
}

// Called when the game starts or when spawned
void ASwitch::BeginPlay()
{
	Super::BeginPlay();

	//on play set type of switch
	SetSwitchType();
	SetSwitchPos();
}

// Called every frame
void ASwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Overridden)
	{
		if (GroupPress)
		{
			//if all three characters are present on the switch
			if (LanosIsOn && SpikeIsOn && MarbIsOn)
			{
				Activated = true;
				if (Base->GetMaterial(0) != greenMat)
				{
					Base->SetMaterial(0, greenMat);
				}
			}
			else
			{
				if (Base->GetMaterial(0) != redMat)
				{
					Base->SetMaterial(0, redMat);
				}
			}
		}
		else if (Timed)
		{
			//decrement time remaining
			ActivatedDuration -= DeltaTime;

			//if time is up
			if (ActivatedDuration <= 0)
			{
				//deactivate switch
				Activated = false;

				if (Base->GetMaterial(0) != redMat)
				{
					Base->SetMaterial(0, redMat);
				}
			}
		}
	}
}

void ASwitch::SetSwitchType()
{
	//on play set type of switch
	switch (SwitchType)
	{
		//requires player to remain on the switch
	case ESwitchEnum::VE_Pressure:
		PressurePad = true;
		Timed = SingleUse = Override = GroupPress = false;
		break;

		//when player presses this switch the door is open for a limited time
	case ESwitchEnum::VE_Timed:
		Timed = true;
		PressurePad = SingleUse = Override = GroupPress = false;
		break;

		//when the player presses this switch, door stays open
	case ESwitchEnum::VE_SingleUse:
		SingleUse = true;
		PressurePad = Timed = Override = GroupPress = false;
		break;

		//will override the other switch attached to door on activation
	case ESwitchEnum::VE_Override:
		Override = true;
		SingleUse = PressurePad = Timed = GroupPress = false;
		break;

		//requires all players to be on a single switch to activate switch
	case ESwitchEnum::VE_GroupPress:
		GroupPress = true;
		SingleUse = PressurePad = Timed = Override = false;
		break;

		//if not set default to single use
	default:
		SingleUse = true;
		PressurePad = Timed = Override = GroupPress = false;
		break;
	}
}

void ASwitch::SetSwitchPos()
{
	switch (SwitchPos)
	{
	case ESwitchPosEnum::VE_OUT:
		Out = true;
		In = false;
		break;
	case ESwitchPosEnum::VE_IN:
		In = true;
		Out = false;
		break;
	default:
		In = true;
		Out = false;
		break;
	}
}


bool ASwitch::GetIsActivated()
{
	return Activated;
}

void ASwitch::OverrideSwitch()
{
	Overridden = true;
}

void ASwitch::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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
				//activate switch
				if (Overridden)
				{
					Activated = true;

					if (Base->GetMaterial(0) != greenMat)
					{
						Base->SetMaterial(0, greenMat);
					}
				}
				else if (GroupPress)
				{
					OnOverlapBeginGroupPress(Ball);
				}
				else
				{
					if (SingleUse && canBePressed)
					{
						//deactivate switch
						if (Activated) {
							Activated = false;
							if (In && Base->GetMaterial(0) != redMat)
							{
								Base->SetMaterial(0, redMat);
							}

							canBePressed = false;
						}
						//activated switch
						else {
							Activated = true;
							if (In && Base->GetMaterial(0) != greenMat)	//Change to green material if an Innie button
							{
								Base->SetMaterial(0, greenMat);
							}

							canBePressed = false;
						}
					}
					else
					{
						//set switch to being pressed
						Activated = true;
						if (Base->GetMaterial(0) != greenMat)
						{
							Base->SetMaterial(0, greenMat);
						}

						//if timed, reinitialise timer
						if (Timed)
							ActivatedDuration = timerDuration;
					}
				}
			}
		}
	}
}

void ASwitch::OnOverlapBeginGroupPress(ABounce_Back_2Ball* const Ball)
{
	if (!LanosIsOn)
	{
		// cast ball to lanos
		ALanos* const Lanos = Cast <ALanos>(Ball);

		//if lanos is on switch
		if (Lanos)
		{
			LanosIsOn = true;
		}
	}

	if (!MarbIsOn)
	{
		// cast ball to Marb
		AMarble* const Marb = Cast <AMarble>(Ball);

		//if marb is on switch
		if (Marb)
		{
			MarbIsOn = true;
		}
	}

	if (!SpikeIsOn)
	{
		// cast ball to Spike
		ASpike* const Spike = Cast <ASpike>(Ball);

		//if marb is on switch
		if (Spike)
		{
			SpikeIsOn = true;
		}
	}
}

void ASwitch::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
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
				if (!Overridden)
				{
					if (GroupPress)
					{
						OnOverlapEndGroupPress(Ball);
					}
					//if pressure pad switch
					else if (PressurePad)
					{
						//set switch to unpressed
						Activated = false;
						Base->SetMaterial(0, redMat);
					}
					else if (SingleUse)
					{
						canBePressed = true;
					}
				}
			}
		}
	}
}

void ASwitch::OnOverlapEndGroupPress(ABounce_Back_2Ball* const Ball)
{
	if (LanosIsOn)
	{
		// cast ball to lanos
		ALanos* const Lanos = Cast <ALanos>(Ball);

		//if lanos is on switch
		if (Lanos)
		{
			LanosIsOn = false;
		}
	}

	if (MarbIsOn)
	{
		// cast ball to Marb
		AMarble* const Marb = Cast <AMarble>(Ball);

		//if marb is on switch
		if (Marb)
		{
			MarbIsOn = false;
		}
	}

	if (SpikeIsOn)
	{
		// cast ball to Spike
		ASpike* const Spike = Cast <ASpike>(Ball);

		//if marb is on switch
		if (Spike)
		{
			SpikeIsOn = false;
		}
	}
}
