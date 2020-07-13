// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Switch.generated.h"

UENUM(BlueprintType)
enum class ESwitchEnum : uint8
{
	VE_Pressure		UMETA(DisplayName = "Pressure"),
	VE_Timed		UMETA(DisplayName = "Timed"),
	VE_SingleUse	UMETA(DisplayName = "SingleUse"),
	VE_Override		UMETA(DisplayName = "Override"),
	VE_GroupPress	UMETA(DisplayName = "GroupPress")
};

UENUM(BlueprintType)
enum class ESwitchPosEnum : uint8
{
	VE_OUT	UMETA(DisplayName = "Out"),
	VE_IN	UMETA(DisplayName = "In")
};

class ABounce_Back_2Ball;
UCLASS()
class BOUNCE_BACK_2_API ASwitch : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//visual representation of the switch's base
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Base;

	//collider for the switch
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

	//switch activation duration
	UPROPERTY(EditAnywhere)
		float timerDuration;

	//sets the type of switch
	UPROPERTY(EditAnywhere)
		ESwitchEnum SwitchType;

	//Determines whether the switch is an innie or outie button
	UPROPERTY(EditAnywhere)
		ESwitchPosEnum SwitchPos;

	//returns if switch has been triggered
	UFUNCTION(BlueprintCallable)
		bool GetIsActivated();

	UFUNCTION()
		void OverrideSwitch();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

private:
	//sets the type of switch
	void SetSwitchType();

	//sets the button of switch
	void SetSwitchPos();

	//does the functionality for group press switch on overlap begin
	void OnOverlapBeginGroupPress(ABounce_Back_2Ball* const Ball);
	//does the functionality for group press switch on overlap end
	void OnOverlapEndGroupPress(ABounce_Back_2Ball* const Ball);

	//flag to represent if the switch is currently active
	bool Activated;

	//flags to represent the type of switch
	bool Timed;
	bool SingleUse;
	bool PressurePad;
	bool Override;
	bool GroupPress;

	//flags to represent the type of button
	bool Out;
	bool In;

	bool LanosIsOn;
	bool SpikeIsOn;
	bool MarbIsOn;

	//signifies if the switches functionality has been overridden
	bool Overridden;

	//siginifies if the single use switch can be pressed again
	bool canBePressed;

	//duration of activation for timer
	float ActivatedDuration;

	//Materials used to make glowing ring change colour
	UMaterial* greenMat;//switch activated
	UMaterial* redMat;//switch deactivated

					  //dynamic material to be changed at runtime
	UMaterialInstanceDynamic* DynMaterial;
};
