// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "Marble.h"
#include "Bounce_Back_2GameMode.h"
#include "Hazards/Pencil.h"
#include "Hazards/Pen.h"

AMarble::AMarble()
{
	// Create mesh component for the model
	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	Model->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Model->SetCastShadow(false);
	Model->SetRelativeScale3D(FVector(1.0f, 1.0, 1.0f));
	Model->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Model->bReceivesDecals = 0;
	Model->SetupAttachment(RootComponent);


	Ball->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	RollTorque = 12000000;
	Ball->SetAngularDamping(0.4f);
	Ball->SetLinearDamping(0.1f);
	Ball->SetPhysicsMaxAngularVelocity(3000.0f);
	Ball->SetRenderInMainPass(false);
}

// Called when the game starts or when spawned
void AMarble::BeginPlay()
{
	Super::BeginPlay();

	gameMode->Marble = this;
}

// Called every frame
void AMarble::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMarble::NotifyActorBeginOverlap (AActor* OtherActor)
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

	// check for Pen
	{
		APen* const Pen = Cast <APen>(OtherActor);

		if (Pen)
		{
			Kill(false);
		}
	}

	if (IsInWater)
	{
		if (Ball->GetMaterial(0) != matWater)
		{
			Ball->SetMaterial(0, matWater);
		}
	}
}

void AMarble::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (!IsInWater)
	{
		if (Ball->GetMaterial(0) != mat)
		{
			Ball->SetMaterial(0, mat);
		}
	}
}

void AMarble::MoveRight(float Val)
{
    ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode> (UGameplayStatics::GetGameMode (GetWorld ()));
    if (GameMode)
    {
        if (!GameMode->GetIsGameplayEnabled ()) return;
    }

	// The speed of the movement (without direction).
	float speed = -1.f * Val * RollTorque;

	// X and Y values of direction used to make 'forward' and 'right' based on camera direction.
	float xSpeed = speed * Direction.X;
	float ySpeed = speed * Direction.Y;


	if (bGrounded)
	{
		const FVector Torque = FVector(xSpeed, ySpeed, 0.f);
		Ball->AddTorque(Torque);
	}
	else
	{
		const FVector Force = FVector(ySpeed * 0.006f, -xSpeed * 0.006f, 0.0f);
		Ball->AddForce(Force);
	}


	RightInputVector.X = Val * sin(-Direction.Y);
	RightInputVector.Y = Val * sin(Direction.X);

	// Spins slower in water
	if (IsInWater)
	{
		//slow Marb in water/glue
		FVector velocity = Ball->GetPhysicsLinearVelocity();
		velocity.X *= 0.98f;
		velocity.Y *= 0.98f;
		Ball->SetPhysicsLinearVelocity(velocity);
	}
}

void AMarble::MoveForward(float Val)
{
    ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode> (UGameplayStatics::GetGameMode (GetWorld ()));
    if (GameMode)
    {
        if (!GameMode->GetIsGameplayEnabled ()) return;
    }

	// The speed of the movement (without direction).
	float speed = Val * RollTorque;

	// X and Y values opposite for forward and backwards movement.
	float xSpeed = speed * -Direction.Y;
	float ySpeed = speed * Direction.X;


	if (bGrounded)
	{
		const FVector Torque = FVector(xSpeed, ySpeed, 0.f);
		Ball->AddTorque(Torque);
	}
	else
	{
		const FVector Force = FVector(ySpeed * 0.006f, -xSpeed * 0.006f, 0.0f);
		Ball->AddForce(Force);
	}


	ForwardInputVector.X = -Val * sin(-Direction.X);
	ForwardInputVector.Y = -Val * sin(-Direction.Y);

	// Spins slower in water
	if (IsInWater)
	{
		//slow Marb in water/glue
		FVector velocity = Ball->GetPhysicsLinearVelocity();
		velocity.X *= 0.98f;
		velocity.Y *= 0.98f;
		Ball->SetPhysicsLinearVelocity(velocity);
	}
}

void AMarble::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMarble::ActionPressed);
}

void AMarble::ActionPressed()
{
    ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode> (UGameplayStatics::GetGameMode (GetWorld ()));
    if (GameMode)
    {
        if (!GameMode->GetIsGameplayEnabled ()) return;
    }

	if (bGrounded && ActionTimer == 0 /*&& !IsInWater*/)
	{
		FVector cameraDirection = Ball->GetComponentLocation() - GetCamera()->GetComponentLocation();
		FVector ballDirection = Ball->GetComponentVelocity();

		FVector currentVelocity = Ball->GetPhysicsLinearVelocity();
		Ball->SetPhysicsLinearVelocity(FVector(0, 0, currentVelocity.Z));

		if (ballDirection.Size2D() == 0)
		{
			Ball->AddForce(cameraDirection.GetSafeNormal2D() * 30000000.0f);
		}
		else if ((ForwardInputVector + RightInputVector).Size() != 0)
		{
			Ball->AddForce((ForwardInputVector.GetSafeNormal2D() + RightInputVector.GetSafeNormal2D() / 2.0f) * 30000000.0f);
		}
		else
		{
			Ball->AddForce(ballDirection.GetSafeNormal2D() * 30000000.0f);
		}

		ActionTimer = 1.0f;

		if (ActionAudio->Sound != nullptr)
		{
			ActionAudio->Play();
		}
	}
}
