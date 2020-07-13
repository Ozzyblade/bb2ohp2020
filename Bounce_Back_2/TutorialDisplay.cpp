// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "TutorialDisplay.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "Components/StaticMeshComponent.h"
#include "TriggerButton.h"

// Sets default values
ATutorialDisplay::ATutorialDisplay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// Create mesh component for the model
	//MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	//RootComponent = MeshComponent;

	//MediaPlayerComponent= CreateDefaultSubobject<UMediaPlayer>(TEXT("MediaPlayer"));;

	bIsPlaying = false;
}

// Called when the game starts or when spawned
void ATutorialDisplay::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void ATutorialDisplay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsPlaying && ThisPlayButton->GetPressed())
	{
		// do something
		if (MediaPlayer)
		{
			bIsPlaying = true;
			MediaPlayerComponent->SetLooping(true);
			MediaPlayerComponent->OpenSource(MediaPlayer);
			MediaPlayerComponent->Play();
		}
	}

}

