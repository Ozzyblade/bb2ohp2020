// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagneticObject.generated.h"

UENUM(BlueprintType)
enum class MagnetType : uint8
{
	VE_FORCE_MOVEABLE UMETA(DisplayName = "FMoveable"),
	VE_MOVEABLE		UMETA(DisplayName = "Moveable"),
	VE_REPULSE		UMETA(DisplayName = "Repulse"),
	VE_ATTRACT		UMETA(DisplayName = "Attract")

};


UCLASS()
class BOUNCE_BACK_2_API AMagneticObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagneticObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Magnetic, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ObjModel;


	UPROPERTY(EditAnywhere, Category = "Components")
		float magneticStrength;

	UPROPERTY(EditAnywhere, Category = "Components")
		MagnetType objType;

	UPROPERTY(EditAnywhere, Category = "Components")
		bool bActive;

	UPROPERTY(EditAnywhere, Category = "Components")
		class ATriggerButton* TriggerButton;

	UPROPERTY(VisibleAnywhere, Category = "Col")
		class UBoxComponent *MagneticZone;

	UPROPERTY(VisibleAnywhere, Category = "Col")
		class UBoxComponent *KillZone;
	

	UFUNCTION()
		void MagneticBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void MagneticEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void KillZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

};
