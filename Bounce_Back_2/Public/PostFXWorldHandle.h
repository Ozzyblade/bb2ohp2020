// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PostFXWorldHandle.generated.h"

class UPostProcessComponent;


UCLASS()
class BOUNCE_BACK_2_API APostFXWorldHandle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APostFXWorldHandle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// This is the post fx used during slowdown
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UPostProcessComponent* PostProcessComponent;
	FPostProcessSettings PostProcessSettings;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "PostFX")
		void ActivateFX();
	UFUNCTION(BlueprintCallable, Category = "PostFX")
		void DeactivateFX();

};
