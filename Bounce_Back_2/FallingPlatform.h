#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bounce_Back_2Ball.h"
#include "FallingPlatform.generated.h"

UCLASS()
class BOUNCE_BACK_2_API AFallingPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFallingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//resets platform so it can fall again
	void reset();

	UFUNCTION()
		void OnOverlap(AActor* pOverlappedActor, AActor* pOtherActor);

	UFUNCTION()
		void OnElapsed();

	//true if platform should collapse when a character moves onto it
	UPROPERTY(EditAnywhere)
		bool Collapses = true;

	//time that must elapse before platform collapses
	UPROPERTY(EditAnywhere)
		float CollapseTime = 1.0f;

	//volume to trigger collapse of platform
	UPROPERTY(EditAnywhere)
		UBoxComponent* TriggerVolume;

	//visual representation of the platform upper pillar
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TrianglesMesh;

	//visual representation of the platform upper pillar
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* UpperPillarMesh;

	//visual representation of the platform lower pillar
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* LowerPillarMesh;

	//audio clip to play when a character moves onto the platform
	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioClip;

	//ensures clip only played once
	bool m_clipPlayed;

private:
	FVector		 m_defaultLocation;
	FQuat		 m_defaultRotation;

	//timer handle for collapse event
	FTimerHandle m_collapseTimer;
};
