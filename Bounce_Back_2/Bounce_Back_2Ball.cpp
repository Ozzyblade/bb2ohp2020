 // Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Bounce_Back_2.h"
#include "Bounce_Back_2Ball.h"
#include "Pickup/Pickup.h"
#include "Pickup/PickupMoondust.h"
#include "Bounce_Back_2GameMode.h"
#include "Hazards/Water.h"
#include "Hazards/FloorHazard.h"
#include "Checkpoint.h"
#include "EngineUtils.h"
#include "GlowSticker.h"
#include "FallingPlatform.h"
#include "Chase.h"
#include "Lanos.h"
#include "Components/PostProcessComponent.h"
#include "Engine/World.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "GameFramework/PlayerController.h"
#include "PostFXWorldHandle.h"
#include "SavePlayerLocation.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "GameFramework/Character.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "DrawDebugHelpers.h"

ABounce_Back_2Ball::ABounce_Back_2Ball()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create mesh component for the ball
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Assets/Meshes/BallMesh.BallMesh"));
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetMobility(EComponentMobility::Static);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.6f);
	Ball->SetLinearDamping(0.2f);
	Ball->BodyInstance.MassScale = 5.0f;
	Ball->BodyInstance.MaxAngularVelocity = 1500.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	Ball->bReceivesDecals = 0;
	RootComponent = Ball;

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	SpringArm->TargetArmLength = InitialCameraDistance;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = true;
	SpringArm->ProbeSize = 20.0f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

											 // Audio components
	ActionAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ActionAudio"));
	ActionAudio->SetupAttachment(RootComponent);
	ActionAudio->bAutoActivate = false;

	MovementAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("MovementAudio"));
	MovementAudio->SetupAttachment(RootComponent);
	MovementAudio->bAutoActivate = false;

	FallingDeathAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("FallingDeathAudio"));
	FallingDeathAudio->SetupAttachment(RootComponent);
	FallingDeathAudio->bAutoActivate = false;

	PencilDeathAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("PencilDeathAudio"));
	PencilDeathAudio->SetupAttachment(RootComponent);
	PencilDeathAudio->bAutoActivate = false;

	RespawnAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("RespawnAudio"));
	RespawnAudio->SetupAttachment(RootComponent);
	RespawnAudio->bAutoActivate = false;

	HitBrickAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("HitBrickAudio"));
	HitBrickAudio->SetupAttachment(RootComponent);
	HitBrickAudio->bAutoActivate = false;

	PickupMoondustAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("PickupMoondustAudio"));
	PickupMoondustAudio->SetupAttachment(RootComponent);
	PickupMoondustAudio->bAutoActivate = false;

	// Create the post fx component
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostFXComponent"));
	PostProcessSettings.bOverride_ColorSaturation = true;

	// Set up some default values
	bGrounded = true;
	FVector const BallPosition = Ball->GetComponentLocation();
	Direction = (BallPosition - Camera->GetComponentLocation()).GetSafeNormal2D();
	IsInWater = false;
	IsGlowing = false;
	hasKey = false;
	hasTrack = false;
	hasWheel = false;
	bPlayedLandingEffect = true;
	Is2D = false;

	IdleCamTime = 10.f;
	bIsIdle = false;
	PreviousVelocity = 0;
	FFMultiplyer = 1;

	BeforeVelocity = FVector(0,0,0);
	AfterVelocity = FVector(0, 0, 0);
	XVel = 0;
	YVel = 0;
	VelThreashold = 50;

	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	Postie = nullptr;

	bUseVelocityChange = false;
	MovementForce = 100.f;
	DesiredDistance = 100.f;
}

// Called when the game starts or when spawned
void ABounce_Back_2Ball::BeginPlay()
{
	Super::BeginPlay();

	Ball->SetSimulatePhysics(true);

	CanSwitch = false;

	gameMode = Cast<ABounce_Back_2GameMode>(GWorld->GetAuthGameMode());

	// Set up some default values
	OriginalLocation = GetActorLocation();
	OriginalRotation = GetActorRotation();
	GetWorldTimerManager().SetTimer(TimerHandle_IdleCamera, this, &ABounce_Back_2Ball::MakeIdle, IdleCamTime, false);

	//If the game starts in 2D
	if (Is2D)
	{
		//Change Camera
		APlayerController* ourPlayer = UGameplayStatics::GetPlayerController(this, 0);
		if (ourPlayer)
		{
			ourPlayer->SetViewTarget(SideCamera);
		}

		//Limit movement in the y-axis
		Ball->BodyInstance.bLockYTranslation = true;
	}

	PostProcessSettings.ColorSaturation.Set(1.0f, 1.0f, 1.0f, 0.1f);
	PostProcessComponent->Settings = PostProcessSettings;
	this->PostProcessComponent->bEnabled = false;

	if (bUsingAI)
		NextPathPoint = GetNextPathPoint();
}

// Called every frame
void ABounce_Back_2Ball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsIdle)
	{
		IdleCameraRotation();
	}

	if (bUsingAI)
	{
		Ball->SetMobility(EComponentMobility::Movable);

		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

		if (DistanceToTarget <= DesiredDistance)
		{
			NextPathPoint = GetNextPathPoint();
		}
		else
		{
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection *= MovementForce;

			// Keep moving to next target
			Ball->AddForce(ForceDirection, NAME_None, bUseVelocityChange);

			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1.f);
		}

		DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Yellow, false, 0.f, 1.f);
	}


	if (Ball->Mobility == EComponentMobility::Movable)
	{
		// Handle camera imformation
		{
			// Edit the camera distance based on speed and some other user defined variables.
			if(!Is2D) SpringArm->TargetArmLength = SpringArm->TargetArmLength - ZoomSpeed * (SpringArm->TargetArmLength - (InitialCameraDistance + Ball->GetComponentVelocity().Size() / (1.0f / ZoomModifier)));

			if (!Is2D)
			{
				// Edit the rotation of the camera based on the direction the player is moving.
				FVector const BallPosition = Ball->GetComponentLocation();
				Direction = (BallPosition - Camera->GetComponentLocation()).GetSafeNormal2D();


				FVector moveDirection = Ball->GetPhysicsLinearVelocity().GetSafeNormal2D();

				FVector velocity = Ball->GetPhysicsLinearVelocity();
				// Code to fix a UE bug that stops sphere physics from being put to sleep (causing slow, constant rolling)
				{
					float magicValue = 6.0f + 60000000.0f / RollTorque;

					if (RightInputVector.Size2D() == 0 && ForwardInputVector.Size2D() == 0)
					{
						if (velocity.Size2D() <= magicValue)
						{
							velocity.X *= 0.0f;
							velocity.Y *= 0.0f;
						}
						else if (velocity.Size2D() <= 100)
						{
							velocity.X *= 0.98f;
							velocity.Y *= 0.98f;
						}
					}

					if (bGrounded && (velocity.Z <= 1 && velocity.Z >= -1))
					{
						Ball->SetPhysicsLinearVelocity(velocity);
					}
				}

				MinimumCamSpeed = Ball->GetPhysicsLinearVelocity().Size2D();

				// Handle the camera timer for analog control
				if (CameraControlTimer != 0)
				{
					CameraRotate = false;
					CameraControlTimer -= DeltaTime;

					if (CameraControlTimer <= 0.0f)
					{
						CameraControlTimer = 0.0f;
					}
				}

				// Set camera rotation control
				if (MinimumCamSpeed <= 200.0f)
				{
					CameraRotate = false;


				}
				else if (CameraControlTimer == 0.0f)
				{
					CameraRotate = true;
				}



				if (!Is2D)
				{
					if (CameraRotate && (moveDirection.GetAbs().X > 0.4f || moveDirection.GetAbs().Y > 0.4f))
					{
						float X = Direction.X + CameraRotateSpeed * (moveDirection.X - Direction.X);
						float Y = Direction.Y + CameraRotateSpeed * (moveDirection.Y - Direction.Y);
						float pitchDiff = -30.0f - CameraRotation.Pitch;
						float pitch = CameraRotation.Pitch + (CameraRotateSpeed / 3) * pitchDiff;

						if (Y >= 0)
						{
							CameraAngle = atan2(Y, X);
							CameraAngle = FMath::RadiansToDegrees(CameraAngle);
						}
						else
						{
							CameraAngle = atan2(-Y, -X);
							CameraAngle = FMath::RadiansToDegrees(CameraAngle) + 180.0f;
						}

						CameraRotation.Pitch = pitch;
						CameraRotation.Roll = 0;
						CameraRotation.Yaw = CameraAngle;

						SpringArm->SetRelativeRotation(CameraRotation);
					}
				}
			}
		}
	}

	if (ActionTimer != 0)
	{
		ActionTimer -= DeltaTime;

		if (ActionTimer < 0.0f)
		{
			ActionTimer = 0.0f;
		}
	}

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);

	FVector Start = Ball->GetComponentLocation();
	FVector End = Start + FVector(0, 0, -100.0f * Ball->GetRelativeTransform().GetScale3D().X);

	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,        //result
		Start,    //start
		End, //end
		ECC_WorldStatic, //collision channel
		RV_TraceParams
	);

	bGrounded = RV_Hit.bBlockingHit;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC != nullptr)
	{

		if (bGrounded && !bPlayedLandingEffect)
		{
			UE_LOG(LogTemp, Warning, TEXT("Prev Velocity: %f"), PreviousVelocity);

			bPlayedLandingEffect = true;

			APlayerController* PC = GetWorld()->GetFirstPlayerController();


			PC->PlayDynamicForceFeedback((-PreviousVelocity / 1000)*FFMultiplyer, 0.1f, true, true, true, true);
		}

		if (bPlayedLandingEffect && !bGrounded)
			bPlayedLandingEffect = false;

		AfterVelocity = BeforeVelocity;
		BeforeVelocity = GetVelocity();

		XVel = BeforeVelocity.X - AfterVelocity.X;
		YVel = BeforeVelocity.Y - AfterVelocity.Y;

		if (XVel < -VelThreashold || XVel > VelThreashold)
		{
			UE_LOG(LogTemp, Warning, TEXT("X Prev Velocity: %f"), BeforeVelocity.X - AfterVelocity.X);

			if (XVel > 1)
				PC->PlayDynamicForceFeedback((XVel / 10)*FFMultiplyer, 0.1f, false, false, true, true);
			else
				PC->PlayDynamicForceFeedback((-XVel / 10)*FFMultiplyer, 0.1f, true, true, false, false);

		}

		if (YVel < -VelThreashold || YVel > VelThreashold)
		{
			UE_LOG(LogTemp, Warning, TEXT("Y Prev Velocity: %f"), BeforeVelocity.Y - AfterVelocity.Y);

			if (YVel > 1)
				PC->PlayDynamicForceFeedback((YVel / 10)*FFMultiplyer, 0.1f, false, false, true, true);
			else
				PC->PlayDynamicForceFeedback((-YVel / 10)*FFMultiplyer, 0.1f, true, true, false, false);
		}
	}

	PreviousVelocity = GetVelocity().Z;


}

void ABounce_Back_2Ball::FellOutOfWorld(UDamageType const & dmgType)
{
	// DON'T CALL Super::FellOutOfWorld (dmgType)!
	// this calls Destroy () which removes the player from the scene
	// and therefore nullifies the code to "kill" then restore the player

	Kill(true);
}

void ABounce_Back_2Ball::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// check for pickup
	{
		// cast actor to APickup
		APickup* const Pickup = Cast <APickup>(OtherActor);

		// if cast is successful and pickup is valid and active
		if (Pickup && !Pickup->IsPendingKill() && Pickup->IsActive())
		{
			// check if pickup is also a battery
			APickupMoondust* const PickupMoondust = Cast <APickupMoondust>(Pickup);
			if (PickupMoondust)
			{
				// call pickup's was collected function
				Pickup->OnCollected();

				// get game mode
				ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				if (GameMode)
				{
					GameMode->UpdateMoondust(PickupMoondust->GetMoondustValue());
				}

				if (PickupMoondustAudio->Sound != nullptr)
				{
					PickupMoondustAudio->Play();
				}

				// deactivate pickup
				Pickup->SetActive(false);
			}
		}
	}

	// check for Water
	{
		if (!IsInWater)
		{
			// cast actor to AWater
			//AWater* const Pencil = Cast <AWater>(OtherActor);
			AFloorHazard* const Pencil = Cast <AFloorHazard>(OtherActor);

			// if cast is successful and pickup is valid and active
			if (Pencil && !Pencil->IsPendingKill())
			{
				IsInWater = true;
			}
		}
	}

	// check for checkpoint
	// performed in Checkpoint_BP
}

void ABounce_Back_2Ball::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	// check for Water
	{
		if (IsInWater)
		{
			// cast actor to AWater
			//AWater* const Pencil = Cast <AWater>(OtherActor);
			AFloorHazard* const Pencil = Cast <AFloorHazard>(OtherActor);

			// if cast is successful and pickup is valid and active
			if (Pencil && !Pencil->IsPendingKill())
			{
				IsInWater = false;
			}
		}
	}
}

void ABounce_Back_2Ball::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	ABlock *block = Cast <ABlock>(Other);
	if (block)
	{
		if (HitBrickAudio->Sound != nullptr)
		{
			HitBrickAudio->Play();
		}
	}
}

void ABounce_Back_2Ball::PossessedBy(class AController *NewController)
{
	this->PostProcessComponent->bEnabled = false;

	Super::PossessedBy(NewController);

	if (!NewController->IsPlayerController()) return;

	Ball->SetMobility(EComponentMobility::Movable);
}

void ABounce_Back_2Ball::UnPossessed()
{
	this->PostProcessComponent->bEnabled = false;

	Super::UnPossessed();

	Ball->SetMobility(EComponentMobility::Static);
}

void ABounce_Back_2Ball::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &ABounce_Back_2Ball::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABounce_Back_2Ball::MoveForward);

	PlayerInputComponent->BindAxis("LookRight", this, &ABounce_Back_2Ball::LookRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ABounce_Back_2Ball::LookUp);

	PlayerInputComponent->BindAction("SwitchToLanos", IE_Pressed, this, &ABounce_Back_2Ball::SwitchToLanos);
	PlayerInputComponent->BindAction("SwitchToSpike", IE_Pressed, this, &ABounce_Back_2Ball::SwitchToSpike);
	PlayerInputComponent->BindAction("SwitchToMarble", IE_Pressed, this, &ABounce_Back_2Ball::SwitchToMarble);
	PlayerInputComponent->BindAction("SwitchToTak", IE_Pressed, this, &ABounce_Back_2Ball::SwitchToMagna);

	PlayerInputComponent->BindAction("CenterCamera", IE_Pressed, this, &ABounce_Back_2Ball::CenterCamera);

	// The action for slowing down time ready for the switching of characters
	PlayerInputComponent->BindAction("LeftBumper", IE_Pressed, this, &ABounce_Back_2Ball::ActivateSlowTime);
	PlayerInputComponent->BindAction("LeftBumper", IE_Released, this, &ABounce_Back_2Ball::DeactivateSlowTime);
	PlayerInputComponent->BindAction("AnyKey", IE_Pressed, this, &ABounce_Back_2Ball::AnyKeyPressed);


	PlayerInputComponent->BindAction("LoadGame", IE_Pressed, this, &ABounce_Back_2Ball::LoadGame);

}

bool ABounce_Back_2Ball::GetCanSwitch() const
{
	return CanSwitch;
}

void ABounce_Back_2Ball::SetIsGlowing(bool val)
{
	IsGlowing = val;
}

bool ABounce_Back_2Ball::IsCurrentlyGlowing()
{
	return IsGlowing;
}

void ABounce_Back_2Ball::SetHasKey()
{
	hasKey = true;
}

bool ABounce_Back_2Ball::GetHasKey()
{
	return hasKey;
}

void ABounce_Back_2Ball::SetHasTrack()
{
	hasTrack = true;
}

bool ABounce_Back_2Ball::GetHasTrack()
{
	return hasTrack;
}

void ABounce_Back_2Ball::SetHasWheel()
{
	hasWheel = true;
}

bool ABounce_Back_2Ball::GetHasWheel()
{
	return hasWheel;
}

void ABounce_Back_2Ball::EnableRoll()
{
	gameMode->SetIsGameplayEnabled(true);
}

void ABounce_Back_2Ball::SwitchCameraTo2D(ACameraActor* camera)
{
	//Change Camera
	APlayerController* ourPlayer = UGameplayStatics::GetPlayerController(this, 0);
	if (ourPlayer)
	{
		ourPlayer->SetViewTargetWithBlend(camera, 1.f);
	}
	
	//Stop Movement
	gameMode->SetIsGameplayEnabled(false);

	//Limit movement in the y-axis
	Ball->BodyInstance.bLockYTranslation = true;

	//Enable Movement
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ABounce_Back_2Ball::EnableRoll, 1.f, false);

	//Limit Movment of Character to 2D Plane
	Is2D = true;

}

void ABounce_Back_2Ball::SwitchCameraTo3D()
{
	//Change Camera
	APlayerController* ourPlayer = UGameplayStatics::GetPlayerController(this, 0);
	if (ourPlayer)
	{
		ourPlayer->SetViewTargetWithBlend(this, 1.f);
	}

	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->SetRelativeLocation(FVector(0.0f));
	Camera->SetRelativeRotation(FRotator(0.0f));

	// Remove limit on the y-axis movment
	Ball->BodyInstance.bLockYTranslation = false;

	// Stop Movement
	gameMode->SetIsGameplayEnabled(false);

	// Enable Movement
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ABounce_Back_2Ball::EnableRoll, 1.f, false);

	//Change Movement back to 3D pespective
	Is2D = false;
}

void ABounce_Back_2Ball::MoveRight(float Val)
{
	ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		if (!GameMode->GetIsGameplayEnabled()) return;
	}

	if (!Is2D)
	{
		// The speed of the movement (without direction).
		float speed = -1.f * Val * RollTorque;

		// X and Y values of direction used to make 'forward' and 'right' based on camera direction.
		float xSpeed = speed * Direction.X;
		float ySpeed = speed * Direction.Y;

		if (bGrounded)
		{
			const FVector Torque = FVector(xSpeed, ySpeed, 0.f);
			//Ball->AddTorque(Torque);
			Ball->AddTorqueInRadians(Torque);
		}
		else
		{
			const FVector Force = FVector(ySpeed * 0.006f, -xSpeed * 0.006f, 0.0f);
			//Ball->AddForce(Force);
			Ball->AddTorqueInRadians(Force);
		}

		RightInputVector.X = Val * sin(-Direction.Y);
		RightInputVector.Y = Val * sin(Direction.X);
	}
	else
	{
		float speed = 1.f * Val * RollTorque;

		if (bGrounded)
		{
			const FVector Torque = FVector(speed, speed, speed) * FVector(0.f, -1.f, 0.f);
			//Ball->AddTorque(Torque);
			Ball->AddTorqueInRadians(Torque);
		}
		else
		{
			float speed = 1.f * Val * RollTorque;
			float xSpeed = speed * Direction.X;
			float ySpeed = speed * Direction.Y;

			// Air Control
			const FVector Force1 = FVector(speed, speed, speed) * FVector(-0.003f, 0.f, 0.f);
			Ball->AddForce(Force1);

			// Air Spin
			const FVector Force = FVector(speed, speed, speed) * FVector(0.f, -.1f, 0.f);
			//Ball->AddTorque(Force);
			Ball->AddTorqueInRadians(Force);
		}
	}
}

void ABounce_Back_2Ball::MoveForward(float Val)
{
	ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		if (!GameMode->GetIsGameplayEnabled()) return;
	}

	if (!Is2D)
	{
		// The speed of the movement (without direction).
		float speed = Val * RollTorque;

		// X and Y values opposite for forward and backwards movement.
		float xSpeed = speed * -Direction.Y;
		float ySpeed = speed * Direction.X;

		if (bGrounded)
		{
			const FVector Torque = FVector(xSpeed, ySpeed, 0.f);
			//Ball->AddTorque(Torque);
			Ball->AddTorqueInRadians(Torque);
		}
		else
		{
			const FVector Force = FVector(ySpeed * 0.006f, -xSpeed * 0.006f, 0.0f);
			//Ball->AddForce(Force);
			Ball->AddTorqueInRadians(Force);
		}

		ForwardInputVector.X = -Val * sin(-Direction.X);
		ForwardInputVector.Y = -Val * sin(-Direction.Y);
	}
	else
	{
		//Do Nothing
	}
}

void ABounce_Back_2Ball::LookRight(float Val)
{
	if (!Is2D)
	{
		if (!bIsIdle)
		{
			ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GameMode)
			{
				if (!GameMode->GetIsGameplayEnabled()) return;
			}

			if (Val != 0.0f)
			{
				CameraControlTimer = 2.0f;
			}

			float speed = 40.0f * Val * CameraRotateSpeed;
			CameraAngle += speed;

			if (CameraAngle < 0)
				CameraAngle += 360;
			if (CameraAngle > 360)
				CameraAngle -= 360;

			CameraRotation.Yaw = CameraAngle;

			SpringArm->SetRelativeRotation(CameraRotation);
		}
	}
}

void ABounce_Back_2Ball::LookUp(float Val)
{
	if (!Is2D)
	{
		if (!bIsIdle)
		{
			ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GameMode)
			{
				if (!GameMode->GetIsGameplayEnabled()) return;
			}

			if (Val != 0.0f)
			{
				CameraControlTimer = 2.0f;
			}

			float speed = InvertedUpCamControl ? 40.0f * -Val * CameraRotateSpeed : 40.0f * Val * CameraRotateSpeed;

			CameraRotation.Pitch += speed;

			if (CameraRotation.Pitch >= 0.0f)
			{
				CameraRotation.Pitch = 0.0f;
			}
			else if (CameraRotation.Pitch <= -80.0f)
			{
				CameraRotation.Pitch = -80.0f;
			}

			SpringArm->SetRelativeRotation(CameraRotation);
		}
	}
}

void ABounce_Back_2Ball::CenterCamera()
{
	// Reset camera
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->SetRelativeLocation(FVector(0.0f));
	Camera->SetRelativeRotation(FRotator(0.0f));
}

// Switching functions

// If the requirements for switching are met then able to switch.
// Even if a character is not unlocked the function to switch to that character is still called but no switch is made.
void ABounce_Back_2Ball::SwitchToLanos()
{
	ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		if (!GameMode->GetIsGameplayEnabled()) return;
	}

	if (gameMode->GetCurrentPlayer() != EPlayerEnum::VE_Lanos && CanSwitch)
	{
		TogglePostFX(false);

		Ball->SetMobility(EComponentMobility::Static);

		CanSwitch = false;
		GetWorldSettings()->SetTimeDilation(1.f);

		gameMode->BecomeLanos();
	}
}

void ABounce_Back_2Ball::SwitchToSpike()
{
	ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		if (!GameMode->GetIsGameplayEnabled()) return;
	}

	if (gameMode->GetCurrentPlayer() != EPlayerEnum::VE_Spike && CanSwitch)
	{
		TogglePostFX(false);

		Ball->SetMobility(EComponentMobility::Static);

		CanSwitch = false;
		GetWorldSettings()->SetTimeDilation(1.f);


		gameMode->BecomeSpike();
	}
}

void ABounce_Back_2Ball::SwitchToMarble()
{
	ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		if (!GameMode->GetIsGameplayEnabled()) return;
	}

	if (gameMode->GetCurrentPlayer() != EPlayerEnum::VE_Marb && CanSwitch && !IsInWater)
	{
		TogglePostFX(false);

		Ball->SetMobility(EComponentMobility::Static);


		CanSwitch = false;
		GetWorldSettings()->SetTimeDilation(1.f);


		gameMode->BecomeMarble();
	}
}

void ABounce_Back_2Ball::SwitchToMagna()
{
	TogglePostFX(false);
	// Tak switching commented out for now
	ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode> (UGameplayStatics::GetGameMode (GetWorld ()));
	if (GameMode)
	{
	if (!GameMode->GetIsGameplayEnabled ()) return;
	}

	if (gameMode->GetCurrentPlayer() != EPlayerEnum::VE_Magna && CanSwitch)
	{
		CanSwitch = false;
		GetWorldSettings()->SetTimeDilation(1.f);


	gameMode->BecomeMagna();
	}
}

void ABounce_Back_2Ball::Kill(bool detachCamera)
{
	// disable gameplay
	ABounce_Back_2GameMode* GameMode = Cast <ABounce_Back_2GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->SetIsGameplayEnabled(false, !detachCamera);
	}

	// disable this character
	SetActorTickEnabled(false); // prevent tick event being called, DOESN'T WORK!

	if (detachCamera)
	{
		if (FallingDeathAudio->Sound != nullptr && !FallingDeathAudio->IsPlaying())
		{
			FallingDeathAudio->Play();
		}

		Camera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
	else
	{
		Ball->SetSimulatePhysics(false);
		Ball->SetVisibility(false, true);

		if (PencilDeathAudio->Sound != nullptr)
		{
			PencilDeathAudio->Play();
		}
	}

	// call respawn in a bit
	if (!RespawnTimer.IsValid())
	{
		GetWorldTimerManager().SetTimer(RespawnTimer, this, &ABounce_Back_2Ball::Respawn, 2.0f);
	}
}

void ABounce_Back_2Ball::Respawn()
{
	// stop respawn timer event from firing again
	GetWorldTimerManager().ClearTimer(RespawnTimer);

	// re-enable gameplay
	ABounce_Back_2GameMode *GameMode = Cast <ABounce_Back_2GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->SetIsGameplayEnabled(true);
	}

	// re-enable this character
	SetActorTickEnabled(true); // re-enable tick event being called, DOESN'T WORK!
	Ball->SetSimulatePhysics(false);
	Ball->SetSimulatePhysics(true);
	Ball->SetVisibility(true, true);
	Ball->SetVisibility(false);

	// Reset camera
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->SetRelativeLocation(FVector(0.0f));
	Camera->SetRelativeRotation(FRotator(0.0f));

	// reset at spawn location
	if (GameMode)
	{
		ACheckpoint *checkpoint = GameMode->GetCurrentCheckpoint();
		FVector new_loc;
		FRotator new_rot;
		if (checkpoint)
		{
			// a player has passed through a checkpoint, so spawn there
			checkpoint->GetSpawnPoint(new_loc, new_rot);
		}
		else
		{
			// no check point has been activated, place back at starting position
			new_loc = OriginalLocation;
			new_rot = OriginalRotation;
		}

		SetActorLocationAndRotation(new_loc, new_rot);
	}

	if (RespawnAudio->Sound != nullptr)
	{
		RespawnAudio->Play();
	}

	//if Lanos is being respawned, reset all glowing stickers
	ALanos* pLanos = Cast <ALanos>(this);
	if (pLanos != nullptr)
	{
		for (TActorIterator<AGlowSticker> stickersIter(GetWorld()); stickersIter; ++stickersIter)
		{
			stickersIter->reset();
		}
	}

	//respawn all falling platforms
	for (TActorIterator<AFallingPlatform> platformsIter(GetWorld()); platformsIter; ++platformsIter)
	{
		platformsIter->reset();
	}

	//reset any chase areas
	for (TActorIterator<AChase> chasesIter(GetWorld()); chasesIter; ++chasesIter)
	{
		chasesIter->reset();
	}

	// force camera behind player
	//SpringArm->SetRelativeRotation (GetActorForwardVector ().Rotation ());
}

void ABounce_Back_2Ball::ActivateSlowTime()
{
	TogglePostFX(true);
	if (Postie)
		Postie->ActivateFX();

	if (bGrounded)
	{
		//Postie->ActivateFX();
		GetWorldSettings()->SetTimeDilation(0.1f);
		CanSwitch = true;
	}
}

void ABounce_Back_2Ball::DeactivateSlowTime()
{
	TogglePostFX(false);
	if (Postie)
		Postie->DeactivateFX();

	if (CanSwitch/* && GetWorldSettings()->GetTimeDilation() == 0.1f*/)
	{
		GetWorldSettings()->SetTimeDilation(1.f);
		CanSwitch = false;
	}
}

void ABounce_Back_2Ball::IdleCameraRotation()
{
	if (!Is2D)
	{
		CameraRotation.Yaw += 0.15f;
		SpringArm->SetRelativeRotation(CameraRotation);
	}
}

void ABounce_Back_2Ball::MakeIdle()
{
	bIsIdle = true;
}

void ABounce_Back_2Ball::AnyKeyPressed()
{
	bIsIdle = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_IdleCamera);
	GetWorldTimerManager().SetTimer(TimerHandle_IdleCamera, this, &ABounce_Back_2Ball::MakeIdle, IdleCamTime, false);
}

void ABounce_Back_2Ball::TogglePostFX(bool val)
{
	if (val)
	{
		this->PostProcessSettings.ColorSaturation.Set(1.0f, 1.0f, 1.0f, 0.1f);
		this->PostProcessComponent->Settings = PostProcessSettings;
	}
	else
	{
		this->PostProcessSettings.ColorSaturation.Set(1.0f, 1.0f, 1.0f, 1.f);
		this->PostProcessComponent->Settings = PostProcessSettings;
	}
}

void ABounce_Back_2Ball::LoadGame()
{
	gameMode->LoadGame();
}

FVector ABounce_Back_2Ball::GetNextPathPoint()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (PlayerPawn)
	{
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetActorLocation(), PlayerPawn->GetTargetLocation());

		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			// Return next point in the path
			return NavPath->PathPoints[1];
		}
	}

	// Failed to find path
	return GetActorLocation();
}