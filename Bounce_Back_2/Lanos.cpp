// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Lanos.h"
#include "Bounce_Back_2GameMode.h"
#include "Hazards/Pencil.h"
#include "Hazards/Pen.h"
#include "Hazards/Water.h"
#include "Hazards/FloorHazard.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/World.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "GameFramework/PlayerController.h"

ALanos::ALanos()
{
	Ball->SetPhysicsMaxAngularVelocity(1500.0f);
	Ball->SetRenderInMainPass(false);

	// Create mesh component for the model
	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	Model->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Model->SetRelativeScale3D(FVector(0.85f, 0.85f, 0.85f));
	Model->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Ball->SetPhysicsMaxAngularVelocity(1500.0f);
	Ball->SetRenderInMainPass(false);
	JumpImpulse = 1200000.0f;

	// Create the shadow decal
	Shadow = CreateDefaultSubobject<UDecalComponent>(TEXT("Shadow"));
	Shadow2 = CreateDefaultSubobject<UDecalComponent>(TEXT("Shadow2"));

	// Set the shadow decal
	Shadow->SetRelativeScale3D(FVector(5.0f, 0.32f, 0.32f));
	Shadow->SetWorldRotation(FVector(0, 0, 180).Rotation());	

	//Shadow2->SetDecalMaterial(material.Object);
	Shadow2->SetRelativeScale3D(FVector(5.0f, 0.32f, 0.32f));
	Shadow2->SetWorldRotation(FVector(0, 0, 180).Rotation());

    Model->bReceivesDecals = 0;
	Model->SetupAttachment(RootComponent);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bCanJump = false;
	IsGlowing = false;
}

// Called when the game starts or when spawned
void ALanos::BeginPlay()
{
	Super::BeginPlay();

	gameMode->Lanos = this;
}

void ALanos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ballPos = GetActorLocation();
	FVector pos = FVector(ballPos.X, ballPos.Y, ballPos.Z - 600.0f);
	Shadow->SetWorldLocation(pos);
	Shadow2->SetWorldLocation(pos);

	Shadow->SetWorldRotation(FVector(0, 0, 180).Rotation());
	Shadow2->SetWorldRotation(FVector(0, 0, 180).Rotation());

}

void ALanos::SetDefaultPhysMaterial()
{
	if(dryMaterial)	Ball->SetPhysMaterialOverride(dryMaterial);
}

void ALanos::NotifyActorBeginOverlap (AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap (OtherActor);

    // check for Pencil
    {
        // cast actor to APencil
        AStationery_Pencil* const Pencil = Cast <AStationery_Pencil> (OtherActor);

        // if cast is successful
        if (Pencil)
        {
            Kill (false);
        }
    }

	//check for Pen
	{
		//cast actor to APen
		APen* const Pen = Cast <APen>(OtherActor);

		//If successful
		if (Pen)
		{
			Kill(false);
		}
	}

	if (IsInWater)
	{
		AFloorHazard* const hazard = Cast<AFloorHazard>(OtherActor);
		if (hazard)
		{
			FVector velocity = Ball->GetPhysicsLinearVelocity();
			//AWater* const col_water = Cast<AWater>(OtherActor);
			//velocity.X *= col_water->getDampening();
			//velocity.Y *= col_water->getDampening();
			velocity.X *= hazard->GetDampening();
			velocity.Y *= hazard->GetDampening();
			Ball->SetPhysicsLinearVelocity(velocity);
			Ball->AddImpulse(velocity);

			if (hazard->GetMaterial())
			{
				Ball->SetPhysMaterialOverride(hazard->GetMaterial());

				FTimerHandle timerHandle;
				GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ALanos::SetDefaultPhysMaterial, 2.f, false);
			}


			RollTorque = 10000000;
		}
	}
}

void ALanos::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (!IsInWater)
	{
		RollTorque = 60000000;
	}
}

void ALanos::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (bounceCount > 3 || bounceCount == 0)
	{
		bCanRoll = true;
	}
	else
	{
		++bounceCount;
	}

	if (bCanJump && bGrounded)
	{
		Jump();
	}
}

void ALanos::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ALanos::ActionPressed);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &ALanos::InAir);


}

void ALanos::ActionPressed()
{
    ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode> (UGameplayStatics::GetGameMode (GetWorld ()));
    if (GameMode)
    {
        if (!GameMode->GetIsGameplayEnabled ()) return;
    }

	bCanJump = true;
}

void ALanos::Jump()
{
	FVector currentVelocity = Ball->GetPhysicsLinearVelocity();
	Ball->SetPhysicsLinearVelocity(FVector(currentVelocity.X, currentVelocity.Y, 0.0f));

	float impulse = JumpImpulse;

	// get game mode
	ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		if (GameMode->GetMoondustCurrent() == 0)
		{
			impulse = InitialJumpImpulse;
		}

		impulse += (JumpAdditionImpulse * GameMode->GetMoondustCurrent());
	}

	if (IsInWater)
	{
		impulse *= 0.5f;
	}

	const FVector Impulse = FVector(0.f, 0.f, impulse);
	Ball->AddImpulse(Impulse);
	bGrounded = false;
	bCanRoll = false;
	bounceCount = 1;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC != nullptr)
	{
		if (GameMode->GetMoondustCurrent() == 0)
			PC->ClientPlayForceFeedback(FFJumpDefault, false, "FF_ButtonClick");
		else
			PC->ClientPlayForceFeedback(FFJumpHigh, false, "FF_ButtonClick");
	}

	if (ActionAudio->Sound != nullptr && !bGrounded)
	{
		ActionAudio->Play();
	}
}

void ALanos::InAir ()
{
    ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode> (UGameplayStatics::GetGameMode (GetWorld ()));
    if (GameMode)
    {
        if (!GameMode->GetIsGameplayEnabled ()) return;
    }

	bCanJump = false;
}

void ALanos::PossessedBy(class AController *NewController)
{
	Super::PossessedBy(NewController);

	if (!NewController->IsPlayerController()) return;
}

void ALanos::UnPossessed()
{
	Super::UnPossessed();

}
