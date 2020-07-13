// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerButton.generated.h"

class UStaticMeshComponent;
class UForceFeedbackEffect;

UENUM(BlueprintType)
enum class ButtonType : uint8
{
	VE_Boolean		UMETA(DisplayName = "Boolean"), // On / Off
	VE_Timed		UMETA(DisplayName = "Timed") // On for a number of seconds
};

UCLASS()
class BOUNCE_BACK_2_API ATriggerButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	float ButtonDepth;

	FVector StartLocalVector;
	FVector LoweredVector;

	UPROPERTY(EditAnywhere, Category = "Components")
	ButtonType ButtonType;

	UPROPERTY(EditAnywhere, Category = "Components")
	float ButtonTime;

	/** The controller force feedback, this could be changed to dynamic */
	UPROPERTY(EditAnywhere, Category = "Components")
	UForceFeedbackEffect* FFButtonClick;

	/** The time it takes to move between clicked and unclicked */
	UPROPERTY(EditAnywhere, Category = "Components")
	float movementDuration;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetPressed() { return bIsPressed; }

	UPROPERTY(BlueprintReadWrite, Category = "Trigger")
	bool bIsPressed;

private:

	bool bWeightOn; // used to decide whether the button should be down or up
	FTimerHandle TimerHandle_ButtonTimeLength;

	void RemovePressed();
	float mRate;
	float total;
};
