// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatorObject.generated.h"

class ATriggerButton;

UENUM(BlueprintType)
enum class ControlType : uint8
{
	VE_Button		UMETA(DisplayName = "Button"), 
	VE_Constant		UMETA(DisplayName = "Constant") 
};

UENUM(BlueprintType)
enum class RotationType : uint8
{
	VE_Linear	UMETA(DisplayName = "Linear"),
	VE_Point	UMETA(DisplayName = "Point")
};

UCLASS()
class BOUNCE_BACK_2_API ARotatorObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatorObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	ControlType ControlType;

	UPROPERTY(EditAnywhere, Category = "Components")
	FVector RotationAngle;

	UPROPERTY(EditAnywhere, Category = "Components")
	RotationType RotationType;

	UPROPERTY(EditAnywhere, Category = "Components")
		FRotator PointRotationAmount;

	UPROPERTY(EditAnywhere, Category = "Components")
	ATriggerButton* TriggerButton;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FRotator StarterRotation;
	FRotator AimedVector;
};
