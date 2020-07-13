// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Bounce_Back_2Ball.h"
#include "SpikeBridge.h"
#include "Hazards/Pencil.h"
#include "Spike.generated.h"

UCLASS()
class BOUNCE_BACK_2_API ASpike : public ABounce_Back_2Ball
{
	GENERATED_BODY()

public:
	ASpike();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

    virtual void PossessedBy (class AController *NewController) override;
    virtual void UnPossessed () override;

	void SpikeAction();

public:
	/** StaticMesh used for the model */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Model;

	/** StaticMesh used for the action model */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ActionModel;

	//The point light that eminates from the center of Spike
	UPROPERTY(EditAnywhere, Category = Ball)
		class UPointLightComponent* m_pSpikeLight;

protected:
    // AActor interface
    virtual void NotifyHit (class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Handle the action. */
	void ActionPressed();

	void Harden();

	void HardenedAction();

	//Used to stop Spike from staying hardened
	float m_hardenControl;

	//Arbitrary time that Spike can stay hardened for
	float m_hardenTime;

	/** Bool for the spikes action cooldown */
	bool spikesOut = false;
	// Bool for whether spike is hardened or not
	bool m_hardened;

	/*void increaseLight();*/

protected:
    /** Vertical impulse to apply when pressing jump */
    UPROPERTY (EditAnywhere, Category = "Spike Destruct", meta = (ClampMin = "0"))
    float DestructibleDamage;

    /** Vertical impulse to apply when pressing jump */
    UPROPERTY (EditAnywhere, Category = "Spike Destruct", meta = (ClampMin = "0"))
    float Radius;

	/** Vertical impulse to apply when pressing jump */
	UPROPERTY(EditAnywhere, Category = "Spike Destruct", meta = (ClampMin = "0"))
	float ImpulseStrength;

    //UPROPERTY (BlueprintReadonly, Category = "Spike Destruct")
	//class URadialForceComponent *special_action;

protected:
    TArray <class ABlock *> blocks;

	void TogglePostFX(bool);
	void ShoulderPressed();
	void ShoulderReleased();

};
