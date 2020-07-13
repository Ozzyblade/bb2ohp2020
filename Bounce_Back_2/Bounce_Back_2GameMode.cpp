// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Bounce_Back_2.h"
#include "Bounce_Back_2GameMode.h"
#include "Bounce_Back_2Ball.h"
#include "Lanos.h"
#include "Spike.h"
#include "Marble.h"
#include "Tak.h"
#include "Checkpoint.h"
#include "UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SavePlayerLocation.h"

ABounce_Back_2GameMode::ABounce_Back_2GameMode ()
{
    // set Moondust
    MoondustCurrent = 0;
    MoondustMax = 4;

    // set variables
    Lanos = nullptr;
    Spike = nullptr;
    Marble = nullptr;
    Magna = nullptr;
    CurrentPlayer = EPlayerEnum::VE_Lanos;

    // set checkpoint
    CurrentCheckpoint = nullptr;
	LanosCheckpoint = nullptr;
	SpikeCheckpoint = nullptr;
	MarbCheckpoint = nullptr;

 // set HUD
    HUDVisiblity = ESlateVisibility::Visible;


    // set dialogue text
    DialogueVisiblity = ESlateVisibility::Hidden;
    DialogueText.Reset ();
    DialogueSpeaker = EPlayerEnum::VE_Lanos;

    IsGameplayEnabled = true;

	bLoadPrevious = false;
	bAutoSave = true;
	AutoSaveTime = 20.0f;
}

void ABounce_Back_2GameMode::BeginPlay ()
{

	if (bAutoSave)
		GetWorldTimerManager().SetTimer(FTimerHandle_AutoSaveTimer, this, &ABounce_Back_2GameMode::SaveGame, AutoSaveTime);


    // Actor has been spawned in the level
    UWorld *world = GetWorld ();
    if (world)
    {
        controller = world->GetFirstPlayerController ();

        if (!controller)
        {
            verifyf (false, TEXT ("GetWorld ()->GetFirstPlayerController () FAILED!"));
        }
    }
    else
    {
        verifyf (false, TEXT ("GetWorld () FAILED!"));
    }

    // set widget variables
    if (HUDWidgetTemplate != nullptr)
    {
        HUDWidget = CreateWidget <UUserWidget> (GetWorld (), HUDWidgetTemplate);
        if (HUDWidget != nullptr)
        {
            HUDWidget->AddToViewport ();
        }
    }
    
   // BecomeLanos();
	BecomeMagna();
    //SetIsGameplayEnabled (false);
}

void ABounce_Back_2GameMode::BecomeLanos ()
{
    if (Lanos != nullptr)
    {
        controller->UnPossess ();
        controller->Possess (Lanos);

        CurrentPlayer = EPlayerEnum::VE_Lanos;
        DisableDialogueText ();
    }
}

void ABounce_Back_2GameMode::BecomeSpike ()
{
    if (Spike != nullptr)
    {
        controller->UnPossess ();
        controller->Possess (Spike);

        CurrentPlayer = EPlayerEnum::VE_Spike;

        DisableDialogueText ();
    }
}

void ABounce_Back_2GameMode::BecomeMarble ()
{
    if (Marble != nullptr)
    {
        controller->UnPossess ();
        controller->Possess (Marble);

        CurrentPlayer = EPlayerEnum::VE_Marb;

        DisableDialogueText ();
    }
}

void ABounce_Back_2GameMode::BecomeMagna ()
{
    if (Magna != nullptr)
    {
        controller->UnPossess ();
        controller->Possess (Magna);

        CurrentPlayer = EPlayerEnum::VE_Magna;

        DisableDialogueText ();
    }
}

bool ABounce_Back_2GameMode::GetIsGameplayEnabled () const
{
    return IsGameplayEnabled;
}

void ABounce_Back_2GameMode::SetIsGameplayEnabled (bool const InIsGameplayEnabled, bool setMobility)
{
    IsGameplayEnabled = InIsGameplayEnabled;

    ABounce_Back_2Ball *player = GetCurrentPlayerPtr ();
    if (player && setMobility)
    {
        if (IsGameplayEnabled)
        {
            player->GetBall ()->SetMobility (EComponentMobility::Movable); // unfreeze mobility
        }
        else
        {
            player->GetBall ()->SetMobility (EComponentMobility::Static); // freeze mobility
        }
    }

}

int32 ABounce_Back_2GameMode::GetMoondustCurrent () const
{
    return MoondustCurrent;
}

int32 ABounce_Back_2GameMode::GetMoondustMax () const
{
    return MoondustMax;
}

void ABounce_Back_2GameMode::UpdateMoondust (int32 const Change)
{
    MoondustCurrent += Change;

    if (MoondustCurrent > MoondustMax)
    {
        MoondustCurrent = MoondustMax;
    }
}

EPlayerEnum ABounce_Back_2GameMode::GetCurrentPlayer () const
{
    return CurrentPlayer;
}

ABounce_Back_2Ball * ABounce_Back_2GameMode::GetCurrentPlayerPtr () const
{
    switch (CurrentPlayer)
    {
    case EPlayerEnum::VE_Lanos: return Cast <ABounce_Back_2Ball> (Lanos); break;
    case EPlayerEnum::VE_Spike: return Cast <ABounce_Back_2Ball> (Spike); break;
    case EPlayerEnum::VE_Marb: return Cast <ABounce_Back_2Ball> (Marble); break;
    case EPlayerEnum::VE_Magna: return Cast <ABounce_Back_2Ball> (Magna); break;
    default:;
    }

    return nullptr;
}

class ACheckpoint * ABounce_Back_2GameMode::GetCurrentCheckpoint () const
{

	if (CurrentPlayer == EPlayerEnum::VE_Lanos)
		return LanosCheckpoint;
	if (CurrentPlayer == EPlayerEnum::VE_Spike)
		return SpikeCheckpoint;
	if (CurrentPlayer == EPlayerEnum::VE_Marb)
		return MarbCheckpoint;

    return CurrentCheckpoint;
}

void ABounce_Back_2GameMode::SetCurrentCheckpoint (ACheckpoint *InCurrentCheckpoint)
{
	if (CurrentPlayer == EPlayerEnum::VE_Lanos)
	{
		LanosCheckpoint = InCurrentCheckpoint;
		
	}																			 //
	else if (CurrentPlayer == EPlayerEnum::VE_Spike)							 //
	{																			 // Make it so that a Character can only spawn at a checkpoint
		SpikeCheckpoint = InCurrentCheckpoint;
																				 // if they reach it themselves
	}																			 //
	else if (CurrentPlayer == EPlayerEnum::VE_Marb)									 //
	{
		MarbCheckpoint = InCurrentCheckpoint;
	}
	else
	CurrentCheckpoint = InCurrentCheckpoint; 
}

bool ABounce_Back_2GameMode::IsCurrentCheckpoint (ACheckpoint* InCurrentCheckpoint) const
{
	if (CurrentPlayer == EPlayerEnum::VE_Lanos)
	{
		return LanosCheckpoint == InCurrentCheckpoint;

	}																			 //
	else if (CurrentPlayer == EPlayerEnum::VE_Spike)							 //
	{																			 // Make it so that a Character can only spawn at a checkpoint
		return SpikeCheckpoint == InCurrentCheckpoint;
		// if they reach it themselves
	}																			 //
	else if (CurrentPlayer == EPlayerEnum::VE_Marb)									 //
	{
		return MarbCheckpoint == InCurrentCheckpoint;
	}
	else
		return CurrentCheckpoint == InCurrentCheckpoint;


    //return CurrentCheckpoint == InCurrentCheckpoint; // I know, but I can't be bothered to do a proper per component check
}

ESlateVisibility ABounce_Back_2GameMode::GetHUDVisibility () const
{
    return HUDVisiblity;
}

void ABounce_Back_2GameMode::SetHUDVisibility (ESlateVisibility const InHUDVisibility)
{
    HUDVisiblity = InHUDVisibility;
}

ESlateVisibility ABounce_Back_2GameMode::GetDialogueVisiblity () const
{
    return DialogueVisiblity;
}

FString ABounce_Back_2GameMode::GetDialogueText () const
{
    return DialogueText;
}

EPlayerEnum ABounce_Back_2GameMode::GetDialogueSpeaker () const
{
    return DialogueSpeaker;
}

void ABounce_Back_2GameMode::EnableDialogueText (FString const & InDialogueText, EPlayerEnum const InDialogueSpeaker)
{
    DialogueVisiblity = ESlateVisibility::Visible;
    DialogueText = InDialogueText;
    DialogueSpeaker = InDialogueSpeaker;
}

void ABounce_Back_2GameMode::DisableDialogueText ()
{
    DialogueVisiblity = ESlateVisibility::Hidden;
}

bool ABounce_Back_2GameMode::ShowFullCharacterWheel () const
{
    return GetPlayerCanSwitch () && GetDialogueVisiblity () == ESlateVisibility::Hidden;
}

bool ABounce_Back_2GameMode::GetPlayerCanSwitch () const
{
    ABounce_Back_2Ball *player = GetCurrentPlayerPtr ();
    if (player)
    {
        return player->GetCanSwitch ();
    }

    return false;
}

// Saving and loading functionality, should be moved to game mode
void ABounce_Back_2GameMode::SaveGame()
{
	// Create instance of save game class 
	USavePlayerLocation* SaveGameInstance = Cast<USavePlayerLocation>(UGameplayStatics::CreateSaveGameObject(USavePlayerLocation::StaticClass()));

	// Set the same game instance location equal to player current location
	SaveGameInstance->LanosLocation = Lanos->GetActorLocation();
	SaveGameInstance->SpikeLocation = Spike->GetActorLocation();
	SaveGameInstance->MarbLocation = Marble->GetActorLocation();
	SaveGameInstance->MagnaLocation = Magna->GetActorLocation();

	// Save the game instance
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("TestSlot"), 0);

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Game Saved"));

	if (bAutoSave)
		GetWorldTimerManager().SetTimer(FTimerHandle_AutoSaveTimer, this, &ABounce_Back_2GameMode::SaveGame, AutoSaveTime);
}

void ABounce_Back_2GameMode::LoadGame()
{
	//USavePlayerLocation* SaveGameInstance = Cast<USavePlayerLocation>(UGameplayStatics::CreateSaveGameObject(USavePlayerLocation::StaticClass()));
	if (USavePlayerLocation* LoadedGame = Cast<USavePlayerLocation>(UGameplayStatics::LoadGameFromSlot(TEXT("TestSlot"), 0)))
	{
		// Set the data from the save game file
		Lanos->SetActorLocation(LoadedGame->LanosLocation);
		Spike->SetActorLocation(LoadedGame->SpikeLocation);
		Marble->SetActorLocation(LoadedGame->MarbLocation);
		Magna->SetActorLocation(LoadedGame->MagnaLocation);

		UE_LOG(LogTemp, Warning, TEXT("LOADED: %s"), *LoadedGame->LanosLocation.ToString());

		// Add a log to show loading
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Game Loaded"));
	}

}