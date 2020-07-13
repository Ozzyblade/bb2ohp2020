// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/Pickup.h"
#include "PickupMoondust.generated.h"

UCLASS()
class BOUNCE_BACK_2_API APickupMoondust : public APickup
{
	GENERATED_BODY()

public:
    APickupMoondust ();

    void OnCollected () override;

    int32 GetMoondustValue () const;
protected:
    UPROPERTY (VisibleAnywhere, BluePrintReadOnly, Category = Moondust, meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent *Mesh;
    UPROPERTY (VisibleAnywhere, BluePrintReadOnly, Category = Moondust, meta = (AllowPrivateAccess = "true"))
    class UParticleSystemComponent *Particles;

    UPROPERTY (VisibleAnywhere, BluePrintReadOnly, Category = Moondust, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent *DetectionArea;

    UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Moondust, meta = (BlueprintProtected = "true"))
    int32 MoondustValue;
};
