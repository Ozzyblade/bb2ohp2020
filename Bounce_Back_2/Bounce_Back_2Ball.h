// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Pawn.h"
#include "Toys/Lego/Block.h"
#include "Bounce_Back_2Ball.generated.h"

class UPostProcessComponent;
class UForceFeedbackEffect;
class APostFXWorldHandle;

UCLASS(config = Game)
class ABounce_Back_2Ball : public APawn
{
	GENERATED_BODY()

public:
	ABounce_Back_2Ball();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// AActor interface
	virtual void FellOutOfWorld(UDamageType const & dmgType) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void PossessedBy(class AController *NewController) override;
	virtual void UnPossessed() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Returns Ball subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	bool GetCanSwitch() const;
	void SetIsGlowing(bool val);
	bool IsCurrentlyGlowing();

	void SetHasKey();
	bool GetHasKey();

	void SetHasTrack();
	bool GetHasTrack();

	void SetHasWheel();
	bool GetHasWheel();

	void SwitchCameraTo2D(ACameraActor* camera);
	void SwitchCameraTo3D();
	void EnableRoll();
	
public:
	UPROPERTY(VisibleAnywhere, Category = Ball)
		float Value = 0;

	//////////////////////
	/// Jump Variables ///
	//////////////////////

	/** Starting vertical jump impulse with no moondyst*/
	UPROPERTY(EditAnywhere, Category = Ball, meta = (ClampMin = "0"))
		float InitialJumpImpulse = 300000.0;

	/** Starting vertical jump impulse after collecting 1 moondust */
	UPROPERTY(EditAnywhere, Category = Ball, meta = (ClampMin = "0"))
		float JumpImpulse = 1200000.0;

	/** The increase in jump per moondust piece */
	UPROPERTY(EditAnywhere, Category = Ball, meta = (ClampMin = "0"))
		float JumpAdditionImpulse = 100000.0;

	/** Indicates whether we can currently jump, use to prevent double jumping */
	UPROPERTY(VisibleAnywhere, Category = Ball)
		bool bGrounded;

	/** Indicates whether we can currently roll using standard torque or using force */
	UPROPERTY(VisibleAnywhere, Category = Ball)
		bool bCanRoll = true;

	//////////////////////////
	/// Movement Variables ///
	//////////////////////////
	/** Torque to apply when trying to roll ball */
	UPROPERTY(EditAnywhere, Category = Ball, meta = (ClampMin = "0"))
		float RollTorque = 60000000.0;

	/** The direction between the camera and the ball */
	UPROPERTY(VisibleAnywhere, Category = Ball)
		FVector Direction;

	/** The direction between the camera and the ball */
	UPROPERTY(VisibleAnywhere, Category = Ball)
		FVector ForwardInputVector;

	/** The direction between the camera and the ball */
	UPROPERTY(VisibleAnywhere, Category = Ball)
		FVector RightInputVector;

	////////////////////////
	/// Camera Variables ///
	////////////////////////
	/** Minimum speed that the camera will start turning automatically */
	UPROPERTY(VisibleAnywhere, Category = Ball)
		float MinimumCamSpeed = 0;

	/** Initial camera distance */
	UPROPERTY(EditAnywhere, Category = Ball, meta = (ClampMin = "0"))
		float InitialCameraDistance = 400;

	/** Camera zoom percentage change per frame between 0 and 1, based on velocity.
	0 = no zoom
	1 = instant zoom*/
	UPROPERTY(EditAnywhere, Category = Ball, meta = (ClampMin = "0", ClampMax = "1"))
		float ZoomSpeed = 0.01f;

	/** Maximum camera zoom out modifier, based on velocity.
	0 = no zoom
	0.5 = half distance
	1 = default distance
	2 = 2 x default distance*/
	UPROPERTY(EditAnywhere, Category = Ball, meta = (ClampMin = "0"))
		float ZoomModifier = 0.5f;

	/** The speed that the camera rotates with the direction of movement of the ball
	0 = no rotation
	1 = instant rotation (not recommended)*/
	UPROPERTY(EditAnywhere, Category = Ball, meta = (ClampMin = "0", ClampMax = "1.0"))
		float CameraRotateSpeed = 0.03f;

	/** Flag to rotate the camera around the ball based on movement direction
	Set to false to disable automatic camera rotations*/
	UPROPERTY(EditAnywhere, Category = Ball)
		bool CameraRotate = true;

	/** Angle of the camera on the z axis */
	UPROPERTY(VisibleAnywhere, Category = Ball)
		float CameraAngle = 0.0f;

	/** Rotation of the camera */
	UPROPERTY(VisibleAnywhere, Category = Ball)
		FRotator CameraRotation;

	UPROPERTY(EditAnywhere, Category = Ball)
		ACameraActor* SideCamera;

	void Kill(bool detachCamera);

protected:
	/** Called for side to side input */
	virtual void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	virtual void MoveForward(float Val);

	/** Called for side to side camera rotation */
	void LookRight(float Val);

	/** Called for side to side camera rotation */
	void LookUp(float Val);

	void CenterCamera();

	void SwitchToLanos();
	void SwitchToSpike();
	void SwitchToMarble();
	void SwitchToMagna();

	// This is the post fx used during slowdown
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UPostProcessComponent* PostProcessComponent;
	FPostProcessSettings PostProcessSettings;

	UPROPERTY(EditAnywhere, Category = "Force Feedback")
	UForceFeedbackEffect* FFGroundBounce;

	UPROPERTY(EditAnywhere, Category = "Post")
	APostFXWorldHandle* Postie;

	UPROPERTY(EditAnywhere, Category = "Force Feedback")
	float FFMultiplyer;

protected:
	/** Variable used for analog camera control */
	UPROPERTY(VisibleAnywhere, Category = Ball)
		float CameraControlTimer = 0.0f;

	/** Variable used for a cooldown timer for the speed boost */
	UPROPERTY(VisibleAnywhere, Category = Ball)
		float ActionTimer = 0.0f;

	/** Variable used for switching character */
	UPROPERTY(VisibleAnywhere, Category = Ball)
		bool CanSwitch = true;

	/** Invert the vertical camera control */
	bool InvertedUpCamControl = true;
	class ABounce_Back_2GameMode* gameMode = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Ball)
		class ABounce_Back_2Ball* lanos = nullptr;

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Ball;

	// bool to show that the character is currently over water
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball)
		bool IsInWater;

	/** Audio component for the action */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* ActionAudio;

	/** Audio component for the character movement*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* MovementAudio;

	/** Audio component for the falling death */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* FallingDeathAudio;

	/** Audio component for the pencil death */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* PencilDeathAudio;

	/** Audio component for the respawn */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* RespawnAudio;

	/** Audio component for the hit brick */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* HitBrickAudio;

	/** Audio component for the pickup of moondust */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* PickupMoondustAudio;

	bool IsGlowing;

	bool hasKey;
	bool hasTrack;
	bool hasWheel;

public:
	UPROPERTY(EditAnywhere, Category = Ball)
		bool Is2D;

	bool GetIs2D() { return Is2D; }
private:
	void Respawn();

	void ActivateSlowTime();
	void DeactivateSlowTime();

private:
	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	// original info for respawning
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		FVector OriginalLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		FRotator OriginalRotation;

	// respawn timer handle
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		FTimerHandle RespawnTimer;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		float IdleCamTime = 10.f;
	bool bIsIdle;
	void IdleCameraRotation();
	void MakeIdle();
	void AnyKeyPressed();
	FTimerHandle TimerHandle_IdleCamera;

	bool bPlayedLandingEffect;
	float PreviousVelocity;

	FVector BeforeVelocity;
	FVector AfterVelocity;
	float XVel,YVel;
	float VelThreashold;
	bool bDisplayPostFX;

	// AI implementation code
public:
	UPROPERTY(EditAnywhere, Category = "BallAI")
	bool bIsPossesed;

	void SetAI(bool Toggle) { bUsingAI = Toggle; }

	UPROPERTY(EditAnywhere, Category = "BallAI")
	bool bUsingAI;

	UPROPERTY(EditDefaultsOnly, Category = "BallAI")
		float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "BallAI")
		bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "BallAI")
		float DesiredDistance;

	FVector GetNextPathPoint();


private:
	// NExt point in the navigation path
	FVector NextPathPoint;

	void TogglePostFX(bool);
	void LoadGame();
};