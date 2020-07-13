// Fill out your copyright notice in the Description page of Project Settings.

#include "Bounce_Back_2.h"
#include "PuzzleBoard.h"
#include "PickupPuzzle.h"
#include "Bounce_Back_2Ball.h"


// Sets default values
APuzzleBoard::APuzzleBoard ()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create mesh component
    static ConstructorHelpers::FObjectFinder <UStaticMesh> PuzzleBoard (TEXT ("/Game/Assets/Meshes/Toys/Puzzle/Toys_Puzzle_PrimitiveHolder.Toys_Puzzle_PrimitiveHolder")); check (PuzzleBoard.Succeeded ());
    Mesh = CreateDefaultSubobject<UStaticMeshComponent> (TEXT ("PuzzleBoard"));
    Mesh->SetStaticMesh (PuzzleBoard.Object);
    Mesh->SetSimulatePhysics (true);
    RootComponent = Mesh;

    static ConstructorHelpers::FObjectFinder <UStaticMesh> Circle (TEXT ("/Game/Assets/Meshes/Toys/Puzzle/Toys_Puzzle_PrimitivePiece_Oval.Toys_Puzzle_PrimitivePiece_Oval")); check (Circle.Succeeded ());
    OvalMesh = CreateDefaultSubobject<UStaticMeshComponent> (TEXT ("Oval"));
    OvalMesh->SetRelativeLocation (FVector (71, -73, 53.5f));
    OvalMesh->SetCollisionProfileName ("NoCollision");
    OvalMesh->SetStaticMesh (Circle.Object);
    OvalMesh->SetVisibility (false);
    OvalMesh->SetupAttachment (RootComponent);

    static ConstructorHelpers::FObjectFinder <UStaticMesh> Triangle (TEXT ("/Game/Assets/Meshes/Toys/Puzzle/Toys_Puzzle_PrimitivePiece_Triangle.Toys_Puzzle_PrimitivePiece_Triangle")); check (Triangle.Succeeded ());
    TriangleMesh = CreateDefaultSubobject<UStaticMeshComponent> (TEXT ("Triangle"));
    TriangleMesh->SetRelativeLocation (FVector (-71, -69, 53.5f));
    TriangleMesh->SetStaticMesh (Triangle.Object);
    TriangleMesh->SetCollisionProfileName ("NoCollision");
    TriangleMesh->SetVisibility (false);
    TriangleMesh->SetupAttachment (RootComponent);

    static ConstructorHelpers::FObjectFinder <UStaticMesh> Square (TEXT ("/Game/Assets/Meshes/Toys/Puzzle/Toys_Puzzle_PrimitivePiece_Square.Toys_Puzzle_PrimitivePiece_Square")); check (Square.Succeeded ());
    SquareMesh = CreateDefaultSubobject<UStaticMeshComponent> (TEXT ("Square"));
    SquareMesh->SetRelativeLocation (FVector (-70, 69, 53.5f));
    SquareMesh->SetStaticMesh (Square.Object);
    SquareMesh->SetCollisionProfileName ("NoCollision");
    SquareMesh->SetVisibility (false);
    SquareMesh->SetupAttachment (RootComponent);

    static ConstructorHelpers::FObjectFinder <UStaticMesh> Hexagon (TEXT ("/Game/Assets/Meshes/Toys/Puzzle/Toys_Puzzle_PrimitivePiece_Hexagon.Toys_Puzzle_PrimitivePiece_Hexagon")); check (Hexagon.Succeeded ());
    HexagonMesh = CreateDefaultSubobject<UStaticMeshComponent> (TEXT ("Hexagon"));
    HexagonMesh->SetRelativeLocation (FVector (70, 69, 53.5f));
    HexagonMesh->SetStaticMesh (Hexagon.Object);
    HexagonMesh->SetCollisionProfileName ("NoCollision");
    HexagonMesh->SetVisibility (false);
    HexagonMesh->SetupAttachment (RootComponent);

    CollisionBox = CreateDefaultSubobject <UBoxComponent> (TEXT ("CollisionBox"));
    CollisionBox->SetRelativeLocationAndRotation (FVector (0, 0, 0), FRotator (0.0f, 0.0f, 0.0f));
    CollisionBox->SetBoxExtent (FVector (200.0f, 200.0f, 50.0f), true);
    CollisionBox->SetupAttachment (RootComponent);

	PlacePuzzleAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("PlacePuzzleAudio"));
	PlacePuzzleAudio->SetupAttachment(RootComponent);
	PlacePuzzleAudio->bAutoActivate = false;
}

bool APuzzleBoard::IsPuzzleComplete ()
{
    if (oval && triangle && square && hexagon)
    {
        activated = true;
    }

    return activated;
}

// Called when the game starts or when spawned
void APuzzleBoard::BeginPlay ()
{
    Super::BeginPlay ();

    if (doorObject != nullptr && doorObject->FindComponentByClass<UStaticMeshComponent> () != nullptr)
    {
        door = doorObject->FindComponentByClass<UStaticMeshComponent> ();
        bool value = false;
        FVector origin;
        FVector box;

        doorObject->GetActorBounds (value, origin, box);

        doorHeight = box.Z * 2.0f;
        startHeight = doorObject->GetActorLocation ().Z;
    }
}

void APuzzleBoard::NotifyActorBeginOverlap(AActor* pOtherActor)
{
	Super::NotifyActorBeginOverlap(pOtherActor);

	//if OtherActor is a Bounce_Back_2Ball
		//if OtherActor is one of the owners of a piece
			//set owner of piece to nullptr
			//set flag for piece collected to true

	// check if the other actor is one of the characters
	ABounce_Back_2Ball* const pCharacter = Cast <ABounce_Back_2Ball>(pOtherActor);
	if (pCharacter != nullptr)
	{
		if (m_pOvalPieceOwner == pCharacter)
			PlaceOval();
		
		if (m_pTrianglePieceOwner == pCharacter)
			PlaceTriangle();

		if (m_pSquarePieceOwner == pCharacter)
			PlaceSquare();

		if (m_pHexagonPieceOwner == pCharacter)
			PlaceHexagon();
	}
}

// Called every frame
void APuzzleBoard::Tick (float DeltaTime)
{
    Super::Tick (DeltaTime);

    if (activated && door != nullptr)
    {
        FVector vec = doorObject->GetActorLocation ();
        vec.Z += DeltaTime * 120.0f;

        doorObject->SetActorLocation (vec);

        if (vec.Z >= startHeight + doorHeight)
        {
            activated = false;
            door->SetMobility (EComponentMobility::Static);
        }
    }
}

void APuzzleBoard::setPieceOwner(PuzzleShape shape, ABounce_Back_2Ball* pCharacter)
{
	switch (shape)
	{
	case PuzzleShape::Oval:
		m_pOvalPieceOwner = pCharacter;
		break;
	case PuzzleShape::Triangle:
		m_pTrianglePieceOwner = pCharacter;
		break;
	case PuzzleShape::Square:
		m_pSquarePieceOwner = pCharacter;
		break;
	case PuzzleShape::Hexagon:
		m_pHexagonPieceOwner = pCharacter;
		break;
	default:
		break;
	}
}

void APuzzleBoard::PlaceOval ()
{
    OvalMesh->SetRelativeLocation (FVector (71, -73, 3.5f));
    OvalMesh->SetVisibility (true);
    OvalMesh->SetCollisionProfileName ("BlockAllDynamic");
    oval = true;
	m_pOvalPieceOwner = nullptr;

	if (PlacePuzzleAudio->Sound != nullptr)
		PlacePuzzleAudio->Play();

    IsPuzzleComplete ();
}

void APuzzleBoard::PlaceTriangle ()
{
    TriangleMesh->SetRelativeLocation (FVector (-71, -69, 3.5f));
    TriangleMesh->SetVisibility (true);
    TriangleMesh->SetCollisionProfileName ("BlockAllDynamic");
    triangle = true;
	m_pTrianglePieceOwner = nullptr;

	if (PlacePuzzleAudio->Sound != nullptr)
		PlacePuzzleAudio->Play();

    IsPuzzleComplete ();
}

void APuzzleBoard::PlaceSquare ()
{
    SquareMesh->SetRelativeLocation (FVector (-70, 69, 3.5f));
    SquareMesh->SetVisibility (true);
    SquareMesh->SetCollisionProfileName ("BlockAllDynamic");
    square = true;
	m_pSquarePieceOwner = nullptr;

	if (PlacePuzzleAudio->Sound != nullptr)
		PlacePuzzleAudio->Play();

    IsPuzzleComplete ();
}

void APuzzleBoard::PlaceHexagon ()
{
    HexagonMesh->SetRelativeLocation (FVector (70, 69, 3.5f));
    HexagonMesh->SetVisibility (true);
    HexagonMesh->SetCollisionProfileName ("BlockAllDynamic");
    hexagon = true;
	m_pHexagonPieceOwner = nullptr;

	if (PlacePuzzleAudio->Sound != nullptr)
		PlacePuzzleAudio->Play();

    IsPuzzleComplete ();
}

