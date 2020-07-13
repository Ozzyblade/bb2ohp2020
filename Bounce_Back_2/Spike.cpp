// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Spike.h"
#include "Bounce_Back_2GameMode.h"
//#include "PhysicsEngine/RadialForceComponent.h"
#include "EngineUtils.h"

ASpike::ASpike ()
{
    // Create mesh component for the model
    Model = CreateDefaultSubobject<UStaticMeshComponent> (TEXT ("Model"));
    Model->BodyInstance.SetCollisionEnabled (ECollisionEnabled::NoCollision);
    Model->SetRelativeScale3D (FVector (2.1f));
	Model->SetCastShadow(false);
    Model->bReceivesDecals = 0;
    Model->SetupAttachment (RootComponent);

    // Create mesh component for the model
    ActionModel = CreateDefaultSubobject<UStaticMeshComponent> (TEXT ("ActionModel"));
    ActionModel->BodyInstance.SetCollisionEnabled (ECollisionEnabled::NoCollision);
    ActionModel->SetRelativeScale3D (FVector (1.2f));
    ActionModel->bReceivesDecals = 0;
	ActionModel->SetCastShadow(false);
    ActionModel->SetupAttachment (RootComponent);

    Ball->SetMassScale ();
    Ball->SetRelativeScale3D (FVector (2.0f));
    Ball->SetPhysicsMaxAngularVelocity (1500.0f);
    Ball->SetRenderInMainPass (false);

	m_pSpikeLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("SpikeLight"));
	m_pSpikeLight->SetRelativeLocation(FVector(0, 0, 0));
	m_pSpikeLight->SetupAttachment(RootComponent);

	// set destructible impulse settings
	DestructibleDamage = 5.0f;
	Radius = 400.0f;
	ImpulseStrength = 200.0f;
	m_hardenControl = 0.0f;
	//IsGlowing = true;
}

// Called when the game starts or when spawned
void ASpike::BeginPlay ()
{
    Super::BeginPlay ();

    gameMode->Spike = this;

    // get blocks
    for (TActorIterator <AActor> ActorItr (GetWorld ()); ActorItr; ++ActorItr)
    {
        ABlock * block = Cast <ABlock> (*ActorItr);
        if (block != nullptr)
        {
            blocks.Add (block);
        }
    }
}

void ASpike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector scale = ActionModel->GetRelativeTransform().GetScale3D();
	if (m_hardened)
	{
		HardenedAction();
	}
	else if (scale.X > 1.2f && !m_hardened)
	{
		scale *= 0.992f;
		ActionModel->SetRelativeScale3D(scale);
	}
	else
	{
		spikesOut = false;
	}

	/*increaseLight();*/

	//Make it so that Spike can't stay hardened forever
	if (m_hardenControl != m_hardenTime)
		m_hardenControl++;
	else
	{
		m_hardenControl = 0.0f;
		m_hardened = false;
	}



}

void ASpike::PossessedBy (class AController *NewController)
{
    Super::PossessedBy (NewController);

    if (!NewController->IsPlayerController ()) return;

    // enable lego blocks
    for (ABlock *block : blocks) block->enable ();
}

void ASpike::UnPossessed ()
{
    Super::UnPossessed ();

    // disable lego blocks
    for (ABlock *block : blocks) block->disable ();
}

void ASpike::NotifyHit (class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit (MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    // send spikes out when touching a block and kill it
    {
		ABlock *block = Cast <ABlock>(Other);
		ASpikeBridge *bridge = Cast <ASpikeBridge>(Other);
		AStationery_Pencil *pencil = Cast <AStationery_Pencil>(Other);
		if (block)
		{
			// send spikes out
			ActionModel->SetRelativeScale3D(FVector(2.0f));

			// set block to kill, causing flashing
			if (!block->GetIsAboutToDie())
			{
				block->Kill();
			}
		}
		if (bridge && (bridge->getFallen() == false))
		{
			//Send notification to Bridge to fall
			ActionModel->SetRelativeScale3D(FVector(2.0f));
			bridge->fall();
			bridge->setFallen(true);
		}
		if (pencil && m_hardened && !pencil->isBlunted())
		{
			pencil->blunted();
		}
    }
}

void ASpike::SetupPlayerInputComponent (class UInputComponent *PlayerInputComponent)
{
    // set up gameplay key bindings
    Super::SetupPlayerInputComponent (PlayerInputComponent);

    PlayerInputComponent->BindAction ("Action", IE_Pressed, this, &ASpike::ActionPressed);
	PlayerInputComponent->BindAction("HardenedAction", IE_Pressed, this, &ASpike::Harden);

	//PlayerInputComponent->BindAction("LeftBumper", IE_Pressed, this, &ASpike::ActivateSlowTime);
	//PlayerInputComponent->BindAction("LeftBumper", IE_Released, this, &ASpike::DeactivateSlowTime);
}

void ASpike::ActionPressed ()
{
    // send spikes out if they are not already out
    if (!spikesOut)
    {
        ActionModel->SetRelativeScale3D (FVector (1.8f));
        spikesOut = true;

		if (ActionAudio->Sound != nullptr)
		{
			ActionAudio->Play();
		}
    }
}

void ASpike::Harden()
{
	m_hardened = true;
	m_hardenTime = 1000.0f;
}

void ASpike::HardenedAction()
{
	ActionModel->SetRelativeScale3D(FVector(2.2f));
}

void ASpike::SpikeAction()
{
	ActionModel->SetRelativeScale3D(FVector(2.0f));
}


void ASpike::ShoulderPressed() {

}

void ASpike::ShoulderReleased() {

}