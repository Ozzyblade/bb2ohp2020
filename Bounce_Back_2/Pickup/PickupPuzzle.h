// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/Pickup.h"
#include "PickupPuzzle.generated.h"

UENUM(BlueprintType)
enum class PuzzleShape : uint8
{
	Hexagon UMETA(DisplayName = "Hexagon"),
    Oval UMETA(DisplayName = "Oval"),
	Triangle UMETA(DisplayName = "Triangle"),
	Square UMETA(DisplayName = "Square")
};

UCLASS()
class BOUNCE_BACK_2_API APickupPuzzle : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupPuzzle();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// To be called when a player collides with the puzzle board
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	// override WasCollected - useImplementation because it's a BlueprintNateEvent
	void OnCollected() override;

	// public way to access the puzzle piece type
	PuzzleShape GetPuzzleShape();

protected:
    // static mesh to represent the pickup in the world
    UPROPERTY (VisibleAnywhere, BluePrintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent *Mesh;

	// set the amount of power the battery gives the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Piece", meta = (BlueprintProtected = "true"))
	PuzzleShape shape = PuzzleShape::Oval;

	UPROPERTY (EditAnywhere, BluePrintReadOnly, Category = "Puzzle Piece")
	class APuzzleBoard* m_pBoard;

	FRotator rotation;

	/** Audio component for the pickup of a puzzle piece */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle Piece", meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* PickupPuzzleAudio;
};