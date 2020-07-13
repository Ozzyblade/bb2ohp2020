// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallingObject.generated.h"

UCLASS()
class BOUNCE_BACK_2_API AFallingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallingObject();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_pModel;

	UPROPERTY(EditAnywhere, Category = Falling)			 //
		float m_ForceX;									 //
														 // Editable forces that enact
	UPROPERTY(EditAnywhere, Category = Falling)			 // on the object and judge the direction
		float m_ForceY;									 // it falls in the scene.
														 //
	UPROPERTY(EditAnywhere, Category = Falling)			 //
		float m_ForceZ;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void fallOver();

		void setFallState(bool s);

		bool getFallState();


private:
	bool m_fallen;



	
};
