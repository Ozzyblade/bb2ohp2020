// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Bounce_Back_2Ball.h"
#include "MagneticObject.h"
#include "Magna.generated.h"

/**
 *
 */
UCLASS()
class BOUNCE_BACK_2_API AMagna : public ABounce_Back_2Ball
{
	GENERATED_BODY()

public:
	AMagna();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// AActor interface
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	virtual void PossessedBy(class AController *NewController) override;
	virtual void UnPossessed() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void addMagneticZone(AMagneticObject* magObj);
	void removeMagneticZone(AMagneticObject* magObj);

private:
	//deal with magnet stuff
	bool bPushing, bPulling; //are we currently in a magnet zone?
	UPROPERTY(EditAnywhere, Category = "Components")
		float magneticPower; //how much force does magna exert on others
	UPROPERTY(EditAnywhere, Category = "Components")
		float distanceMulti; //how much stronger does the force get as we get closer to an interactable object




	void pushAction() { bPushing = true; }
	void pullAction() { bPulling = true; }
	void stopPullAction() { bPulling = false; }
	void stoppushAction() { bPushing = false; }

	TArray<AMagneticObject*> magneticZonesActive;
protected:
	/** StaticMesh used for the model */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Model;
};
