// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallBridge.generated.h"

UCLASS()
class BOUNCE_BACK_2_API ABallBridge : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABallBridge();

	UPROPERTY(EditAnywhere)
		UBoxComponent* m_pFallTrigger;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_pModel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	UPROPERTY(EditAnywhere, Category = Fall)	  //
		float m_ForceX;							  //
												  // Editable forces that judge
	UPROPERTY(EditAnywhere, Category = Fall)	  // the direction the bridge falls
		float m_ForceY;							  //
												  //
	UPROPERTY(EditAnywhere, Category = Fall)	  //
		float m_ForceZ;							  //

	void fall();

	void setFallen(bool state);

	bool getFallen();

private:

	bool m_Fallen;
};
