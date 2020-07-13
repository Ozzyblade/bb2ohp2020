// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/GameMode.h"
#include "SlateWrapperTypes.h"
#include "Bounce_Back_2GameMode.generated.h"

class USavePlayerLocation;

UENUM(BlueprintType)
enum class EPlayerEnum : uint8
{
    VE_Lanos UMETA (DisplayName = "Lanos"),
    VE_Spike UMETA (DisplayName = "Spike"),
    VE_Marb  UMETA (DisplayName = "Marb"),
    VE_Magna UMETA (DisplayName = "Magna")
};

UCLASS(minimalapi)
class ABounce_Back_2GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABounce_Back_2GameMode();

	virtual void BeginPlay() override;

	void BecomeLanos();
	void BecomeSpike();
	void BecomeMarble();
	void BecomeMagna();

    // functions for enabling/disabling game
    UFUNCTION (BlueprintCallable, Category = Game)
    bool GetIsGameplayEnabled () const;
    UFUNCTION (BlueprintCallable, Category = Game)
    void SetIsGameplayEnabled (bool const InIsGameplayEnabled, bool setMobility = true);
    
    // accessor function for MoondustCurrent
    UFUNCTION (BlueprintCallable, Category = Moondust)
    int32 GetMoondustCurrent () const;

    // accessor function for MoondustCurrent
    UFUNCTION (BlueprintCallable, Category = Moondust)
    int32 GetMoondustMax () const;

    // update moondust value
    UFUNCTION (BlueprintCallable, Category = Moondust)
    void UpdateMoondust (int32 const Change);

    // accessor function for CurrentPlayer
    UFUNCTION (BlueprintCallable, Category = Player)
    EPlayerEnum GetCurrentPlayer () const;

    UFUNCTION (BlueprintCallable, Category = Player)
    class ABounce_Back_2Ball * GetCurrentPlayerPtr () const;

    // accessor function for CurrentCheckpoint
    UFUNCTION (BlueprintCallable, Category = Checkpoint)
    class ACheckpoint* GetCurrentCheckpoint () const;
    UFUNCTION (BlueprintCallable, Category = Checkpoint)
    void SetCurrentCheckpoint (class ACheckpoint* InCurrentCheckpoint);
    UFUNCTION (BlueprintCallable, Category = Checkpoint)
    bool IsCurrentCheckpoint (class ACheckpoint* InCurrentCheckpoint) const;
    
    // accessor function for setting HUDs visibility
    UFUNCTION (BlueprintCallable, Category = HUD)
    ESlateVisibility GetHUDVisibility () const;
    UFUNCTION (BlueprintCallable, Category = HUD)
    void SetHUDVisibility (ESlateVisibility const InHUDVisibility);

    // functions to control dialogue Dialogue
    UFUNCTION (BlueprintCallable, Category = Dialogue)
    ESlateVisibility GetDialogueVisiblity () const;
    UFUNCTION (BlueprintCallable, Category = Dialogue)
    FString GetDialogueText () const;
    UFUNCTION (BlueprintCallable, Category = Dialogue)
    EPlayerEnum GetDialogueSpeaker () const;

    UFUNCTION (BlueprintCallable, Category = Dialogue)
    void EnableDialogueText (FString const & InDialogueText, EPlayerEnum const InDialogueSpeaker);
    UFUNCTION (BlueprintCallable, Category = Dialogue)
    void DisableDialogueText ();

    UFUNCTION (BlueprintCallable, Category = Ball)
    bool ShowFullCharacterWheel () const;

    UFUNCTION (BlueprintCallable, Category = Ball)
    bool GetPlayerCanSwitch () const;
public:
	class ALanos *Lanos;
	class ASpike *Spike;
	class AMarble *Marble;
	class ABounce_Back_2Ball *Magna;

	bool POSTFXBUGFIX;

protected:
    // widget class to use for HUD screen
    UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = Power, meta = (BlueprintProtected = "true"))
    TSubclassOf <class UUserWidget> HUDWidgetTemplate;

    // actual instance of the HUD
    UPROPERTY ()
    class UUserWidget* HUDWidget;



private:

private:
	APlayerController* controller = nullptr;
    
    // the moondust level
    UPROPERTY (VisibleAnywhere, Category = Moondust)
    int32 MoondustCurrent;

    // the maximium  level
    UPROPERTY (VisibleAnywhere, Category = Moondust)
    int32 MoondustMax;
    
    // current player
	UPROPERTY (VisibleAnywhere, Category = Player)
	EPlayerEnum CurrentPlayer;
    
    // checkpoint to spawn at if player dies
	UPROPERTY (VisibleAnywhere, Category = Checkpoint)
	class ACheckpoint *CurrentCheckpoint;

	// checkpoint to spawn at if player dies
	UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		class ACheckpoint *LanosCheckpoint;

	// checkpoint to spawn at if player dies
	UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		class ACheckpoint *SpikeCheckpoint;

	// checkpoint to spawn at if player dies
	UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		class ACheckpoint *MarbCheckpoint;

    // HUD
    ESlateVisibility HUDVisiblity;

    // dialogue text
    ESlateVisibility DialogueVisiblity;
    FString DialogueText;
    EPlayerEnum DialogueSpeaker;

	bool IsGameplayEnabled;

protected:
	UPROPERTY(EditAnywhere, Category = "Save")
	bool bAutoSave;

	UPROPERTY(EditAnywhere, Category = "Save")
	bool bLoadPrevious;

	UPROPERTY(EditAnywhere, Category = "Save")
	float AutoSaveTime;

public:
	void SaveGame();
	void LoadGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	FTimerHandle FTimerHandle_AutoSaveTimer;
};
