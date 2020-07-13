// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PickupPuzzle.h"
#include "PuzzleBoard.generated.h"

UCLASS()
class BOUNCE_BACK_2_API APuzzleBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleBoard();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// To be called when a player collides with the puzzle board
	virtual void NotifyActorBeginOverlap(AActor* pOtherActor) override;
	
	// Called every frame
	virtual void Tick( float deltaSeconds ) override;

	// Notify PuzzleBoard which character owns the piece now
	void setPieceOwner(PuzzleShape shape, class ABounce_Back_2Ball* pCharacter);

	void PlaceOval ();
	void PlaceTriangle();
	void PlaceSquare();
	void PlaceHexagon();

protected:
	UPROPERTY(VisibleAnywhere, Category = "PuzzleBoard")
	class UStaticMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere, Category = "PuzzleBoard")
	class UBoxComponent *CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "PuzzleBoard")
	class UStaticMeshComponent *OvalMesh;

	UPROPERTY(VisibleAnywhere, Category = "PuzzleBoard")
	class UStaticMeshComponent *TriangleMesh;

	UPROPERTY(VisibleAnywhere, Category = "PuzzleBoard")
	class UStaticMeshComponent *SquareMesh;

	UPROPERTY(VisibleAnywhere, Category = "PuzzleBoard")
	class UStaticMeshComponent *HexagonMesh;

	// Check if the puzzle has been completed.
	UFUNCTION(BlueprintCallable, Category = Game)
	bool IsPuzzleComplete();

	UPROPERTY(EditAnywhere, Category = "PuzzleBoard")
	class AActor *doorObject = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "PuzzleBoard")
	class UStaticMeshComponent *door = nullptr;

	float doorHeight = 0.0f;
	float startHeight = 0.0f;

	bool oval = false;
	bool triangle = false;
	bool square = false;
	bool hexagon = false;
	bool activated = false;

	class ABounce_Back_2Ball* m_pOvalPieceOwner		= nullptr;
	class ABounce_Back_2Ball* m_pTrianglePieceOwner	= nullptr;
	class ABounce_Back_2Ball* m_pSquarePieceOwner	= nullptr;
	class ABounce_Back_2Ball* m_pHexagonPieceOwner	= nullptr;

	/** Audio component for the placement of a puzzle piece */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PuzzleBoard", meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* PlacePuzzleAudio;
};
