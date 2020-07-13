// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialDisplay.generated.h"

class UMediaPlayer;
class UStaticMesh;
class ATriggerButton;
class UMediaSource;

UCLASS()
class BOUNCE_BACK_2_API ATutorialDisplay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialDisplay();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The mesh of the media player
	//UPROPERTY(EditAnywhere, Category = "Components")
	//UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UMediaPlayer* MediaPlayerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMediaSource* MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	ATriggerButton* ThisPlayButton;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	bool bIsPlaying;

};
