// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagneticObject.h"
#include "Screw_Hazard.generated.h"

/**
 * 
 */
UCLASS()
class BOUNCE_BACK_2_API AScrew_Hazard : public AMagneticObject
{
	GENERATED_BODY()
public:

	AScrew_Hazard();

protected:
	virtual void BeginPlay() override;
	
};
