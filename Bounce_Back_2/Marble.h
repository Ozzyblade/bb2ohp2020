// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Bounce_Back_2Ball.h"
#include "Marble.generated.h"

/**
 * 
 */
UCLASS()
class BOUNCE_BACK_2_API AMarble : public ABounce_Back_2Ball
{
	GENERATED_BODY()
	
public:
	AMarble();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

    // AActor interface
    virtual void NotifyActorBeginOverlap (AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

protected:
	/** StaticMesh used for the model */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Model;

	/** Handle the action. */
	void ActionPressed();

	/** Called for side to side input */
	virtual void MoveRight(float Val) override;

	/** Called to move ball forwards and backwards */
	virtual void MoveForward(float Val) override;

	UMaterial* mat;
	UMaterial* matWater;

	UMaterial* glowingMat;//glowing cateyes

	//light for marb
	UPROPERTY(EditAnywhere)
		UPointLightComponent* marbsLight;

};
