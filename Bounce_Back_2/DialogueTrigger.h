// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "Bounce_Back_2GameMode.h"
#include "DialogueTrigger.generated.h"

UENUM (BlueprintType, meta = (Bitflags))
enum class DialogueTriggerTriggerer : uint8
{
    LANOS UMETA (DisplayName = "Lanos"),
    SPIKE UMETA (DisplayName = "Spike"),
    MARB  UMETA (DisplayName = "Marb"),
    Magna   UMETA (DisplayName = "Magna")
};
ENUM_CLASS_FLAGS (DialogueTriggerTriggerer)

UENUM (BlueprintType)
enum class DialogueTriggerSpeaker : uint8
{
    TRIGGERER UMETA (DisplayName = "Triggerer"),
    LANOS     UMETA (DisplayName = "Lanos"),
    SPIKE     UMETA (DisplayName = "Spike"),
    MARB      UMETA (DisplayName = "Marb"),
	Magna       UMETA (DisplayName = "Magna")
};

UCLASS()
class BOUNCE_BACK_2_API ADialogueTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueTrigger();

    // AActor interface
    virtual void NotifyActorBeginOverlap (AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap (AActor* OtherActor) override;

public:
    UPROPERTY (EditAnywhere, Category = Dialogue, meta = (Bitmask, BitmaskEnum = DialogueTriggerTriggerer))
    uint8 TriggerMask;

    UPROPERTY (EditAnywhere, Category = Dialogue)
    TEnumAsByte <DialogueTriggerSpeaker> Speaker;

    // the text to display
    UPROPERTY (EditAnywhere, Category = Dialogue)
    FString Text;

    // only trigger the dialogue box once
    UPROPERTY (EditAnywhere, Category = Dialogue)
    bool DoOnce;

private:
    bool IsActiveOverlap (AActor *OtherActor, EPlayerEnum & DialogueSpeaker);

private:
    bool DialogueAlreadyTriggered;

};
