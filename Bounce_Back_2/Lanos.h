// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Bounce_Back_2Ball.h"
#include "Lanos.generated.h"

class UForceFeedbackEffect;

/**
*
*/
UCLASS()
class BOUNCE_BACK_2_API ALanos : public ABounce_Back_2Ball
{
	GENERATED_BODY()

public:
	ALanos();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

    // AActor interface
    virtual void NotifyActorBeginOverlap (AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void NotifyHit (class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	UPROPERTY(VisibleAnywhere, Category = Ball)
	bool bCanJump = false;

	virtual void PossessedBy(class AController *NewController) override;
	virtual void UnPossessed() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	UForceFeedbackEffect* FFJumpDefault;

	UPROPERTY(EditAnywhere, Category = "Components")
	UForceFeedbackEffect* FFJumpHigh;

protected:
	/** Handle the action. */
	void ActionPressed();

	/** Handle the jump */
	void Jump();

	/** Handles input for potential next jump while in air. */
	void InAir();

	/** Change the material back to defualt one*/
	void SetDefaultPhysMaterial();

protected:
	/** StaticMesh used for the model */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Model;

	int bounceCount = 0;
private:
	/** Main shadow decal for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* Shadow;

	/** Secondary shadow decal for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* Shadow2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UPhysicalMaterial* wetMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicalMaterial* modifiedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicalMaterial* dryMaterial;

public:
	FORCEINLINE class UStaticMeshComponent* GetModel() const { return Model; }

};
