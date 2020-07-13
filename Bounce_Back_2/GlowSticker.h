// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GlowSticker.generated.h"

UCLASS()
class BOUNCE_BACK_2_API AGlowSticker : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGlowSticker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//resets sticker as if it was never picked up
	void reset();

	UFUNCTION()
		void OnOverlap(AActor* pOverlappedActor, AActor* pOtherActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* m_pMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPointLightComponent* m_pLight;

private:
	UPROPERTY()
		AActor*		m_pParentActor;

	UPROPERTY()
		bool		m_waitingToDetach;

	UPROPERTY(EditAnywhere, Category = Stickiness)
		float		m_totalStickTime;

	UPROPERTY(EditAnywhere, Category = Stickiness)
		float		m_stickTimeRemaining;

	// There will be a period of time when the sticker is detached from the player
	// but cannot be picked up again while the sticker reorientates to the landscape.
	// When coming to a rest, it will be possible to stick to players again.
	UPROPERTY()
		bool		m_canPickup;

	FVector			m_defaultLocation;

	static int		numOfStickers;
};
