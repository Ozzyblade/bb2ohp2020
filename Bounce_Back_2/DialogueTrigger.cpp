// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "DialogueTrigger.h"
#include "Lanos.h"
#include "Spike.h"
#include "Marble.h"
#include "Tak.h"

ADialogueTrigger::ADialogueTrigger()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // set variables
    TriggerMask = 0;
    Speaker = DialogueTriggerSpeaker::TRIGGERER;
    Text.Reset ();
    DoOnce = true;
    DialogueAlreadyTriggered = false;
}

void ADialogueTrigger::NotifyActorBeginOverlap (AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap (OtherActor);

    EPlayerEnum NewSpeaker;
    if (IsActiveOverlap (OtherActor, NewSpeaker))
    {
        // get game mode
        ABounce_Back_2GameMode *gamemode = Cast <ABounce_Back_2GameMode> (GWorld->GetAuthGameMode ());
        if (gamemode)
        {
            if (DoOnce)
            {
                if (DialogueAlreadyTriggered) return;
                DialogueAlreadyTriggered = true;
            }

            gamemode->EnableDialogueText (Text, NewSpeaker);
        }
    }
}

void ADialogueTrigger::NotifyActorEndOverlap (AActor* OtherActor)
{
    Super::NotifyActorEndOverlap (OtherActor);

    EPlayerEnum NewSpeaker;
    if (IsActiveOverlap (OtherActor, NewSpeaker))
    {
        // get game mode
        ABounce_Back_2GameMode *gamemode = Cast<ABounce_Back_2GameMode> (GWorld->GetAuthGameMode ());

        if (gamemode)
        {
            gamemode->DisableDialogueText ();
        }
    }
}

bool ADialogueTrigger::IsActiveOverlap (AActor *OtherActor, EPlayerEnum & DialogueSpeaker)
{
    switch (Speaker)
    {
    case DialogueTriggerSpeaker::LANOS: DialogueSpeaker = EPlayerEnum::VE_Lanos; break;
    case DialogueTriggerSpeaker::SPIKE: DialogueSpeaker = EPlayerEnum::VE_Spike; break;
    case DialogueTriggerSpeaker::MARB: DialogueSpeaker = EPlayerEnum::VE_Marb; break;
    //case DialogueTriggerSpeaker::MAGNA: DialogueSpeaker = EPlayerEnum::VE_Magna; break;

    case DialogueTriggerSpeaker::TRIGGERER:
    default:;
    }

    // check for Lanos
    {
        // cast actor to ALanos
        ALanos* const lanos = Cast <ALanos> (OtherActor);

        // if cast is successful and pickup is valid and active
        if (lanos && !lanos->IsPendingKill ())
        {
            if ((TriggerMask & (1 << static_cast <uint32> (DialogueTriggerTriggerer::LANOS))) > 0)
            {
                if (Speaker == DialogueTriggerSpeaker::TRIGGERER) DialogueSpeaker = EPlayerEnum::VE_Lanos;

                return true;
            }
        }
    }

    // check for Spike
    {
        // cast actor to ASpike
        ASpike* const spike = Cast <ASpike> (OtherActor);

        // if cast is successful and pickup is valid and active
        if (spike && !spike->IsPendingKill ())
        {
            if ((TriggerMask & (1 << static_cast <uint32> (DialogueTriggerTriggerer::SPIKE))) > 0)
            {
                if (Speaker == DialogueTriggerSpeaker::TRIGGERER) DialogueSpeaker = EPlayerEnum::VE_Spike;

                return true;
            }
        }
    }

    // check for Marb
    {
        // cast actor to ASpike
        AMarble* const marb = Cast <AMarble> (OtherActor);

        // if cast is successful and pickup is valid and active
        if (marb && !marb->IsPendingKill ())
        {
            if ((TriggerMask & (1 << static_cast <uint32> (DialogueTriggerTriggerer::MARB))) > 0)
            {
                if (Speaker == DialogueTriggerSpeaker::TRIGGERER) DialogueSpeaker = EPlayerEnum::VE_Marb;

                return true;
            }
        }
    }

    //// check for Tak
    //{
    //    // cast actor to ATak
    //    ATak* const tak = Cast <ATak> (OtherActor);

    //    // if cast is successful and pickup is valid and active
    //    if (tak && !tak->IsPendingKill ())
    //    {
    //        if ((TriggerMask & (1 << static_cast <uint32> (DialogueTriggerTriggerer::TAK))) > 0)
    //        {
    //            if (Speaker == DialogueTriggerSpeaker::TRIGGERER) DialogueSpeaker = EPlayerEnum::VE_Tak;

    //            return true;
    //        }
    //    }
    //}

    return false;
}
