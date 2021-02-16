//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "ChunkGenerator.h"
//#include "ProceduralMeshComponent.h"
//#include "Chunk.generated.h"
//
//UENUM(BlueprintType)
//enum class EBlockType : uint8 {
//	AIR UMETA(DisplayName = "AirBlock"),
//	DIRT UMETA(DisplayName = "DirtBlock"),
//	STONE UMETA(DisplayName = "StoneBlock"),
//	SAND UMETA(DisplayName = "SandBlock"),
//	UNBREAKABLE UMETA(DisplayName = "UnbreakableBlock")
//};
//
//UENUM(BlueprintType)
//enum class EFaceDir : uint8 {
//	UP UMETA(DisplayName = "Up"),
//	DOWN UMETA(DisplayName = "Down"),
//	LEFT UMETA(DisplayName = "Left"),
//	RIGHT UMETA(DisplayName = "Right"),
//	FRONT UMETA(DisplayName = "Front"),
//	BACK UMETA(DisplayName = "Back")
//};
//
//UCLASS()
//class CS378_FINAL_API AChunk : public AActor
//{
//	GENERATED_BODY()
//
//public:
//	// Sets default values for this actor's properties
//	AChunk();
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//		UProceduralMeshComponent* ChunkMesh;
//
//	static const int blockSize = 100;
//	static const int width = 16;
//	static const int height = 96;
//
//	// 0 = air, 1 = dirt, ...
//	// At lowest height, generate unbreakable
//	static TMap<AChunkGenerator::ChunkPos, EBlockType* [width + 2][height]> allChunkInfo;
//	static TMap<AChunkGenerator::ChunkPos, int> curMeshes; // Maps chunk pos to procedural mesh section index
//	static TArray<int> availableIndices;
//
//	void SetRenderDist(float renderDist);
//	void GenMesh(AChunkGenerator::ChunkPos cp);
//
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//	static TArray<FVector> Vertices;
//	static TArray<int32> Triangles;
//	static TArray<FVector2D> UVs;
//
//	// Adds a face based on parameters, index is face number
//	void AddFace(FVector blockPos, EFaceDir faceDir, EBlockType blockType, int index);
//
//public:
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//};
