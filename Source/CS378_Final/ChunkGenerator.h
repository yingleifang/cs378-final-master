// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "Components/RuntimeMeshComponentStatic.h"
#include "ChunkGenerator.generated.h"


UENUM(BlueprintType)
enum class EBlockType : uint8 {
	AIR UMETA(DisplayName = "AirBlock"),
	DIRT UMETA(DisplayName = "DirtBlock"),
	STONE UMETA(DisplayName = "StoneBlock"),
	SAND UMETA(DisplayName = "SandBlock"),
	UNBREAKABLE UMETA(DisplayName = "UnbreakableBlock")
};

UENUM(BlueprintType)
enum class EFaceDir : uint8 {
	UP UMETA(DisplayName = "Up"),
	DOWN UMETA(DisplayName = "Down"),
	LEFT UMETA(DisplayName = "Left"),
	RIGHT UMETA(DisplayName = "Right"),
	FRONT UMETA(DisplayName = "Front"),
	BACK UMETA(DisplayName = "Back")
};

UCLASS()
class CS378_FINAL_API AChunkGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChunkGenerator();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		URuntimeMeshComponentStatic* ChunkMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* ChunkMeshMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int renderDist = 3;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float renderChunkDelay = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float destroyChunkDelay = 0.05f;*/

	const float uvMod = 4;
	static const int blockSize = 100;
	static const int width = 16;
	static const int height = 48;

	struct ChunkPos {
	public:
		int x, y;
		ChunkPos(){}
		ChunkPos(int x_, int y_) : x(x_), y(y_) {}

		friend uint32 GetTypeHash(const ChunkPos& Other) {
			uint64 hash = ((((uint64)Other.x * 8317 + (uint64)Other.y * 821) / 6199) * 8039) % 2006009;
			return GetTypeHash((uint32)hash);
		}
		friend bool operator==(const ChunkPos& cp1, const ChunkPos& cp2) {
			return (cp1.x == cp2.x && cp1.y == cp2.y);
		}
		ChunkPos& operator=(ChunkPos cp) {
			x = cp.x;
			y = cp.y;
			return *this;
		}
		friend ChunkPos operator+(const ChunkPos& cp1, const ChunkPos& cp2) {
			return ChunkPos(cp1.x + cp2.x, cp1.y + cp2.y);
		}
		friend ChunkPos operator-(const ChunkPos& cp1, const ChunkPos& cp2) {
			return ChunkPos(cp1.x - cp2.x, cp1.y - cp2.y);
		}
	};

	struct ChunkInfo {
	public:
		EBlockType data[width + 2][width + 2][height];
		FVector worldOffset;
		ChunkInfo(FVector worldOffset_) : worldOffset(worldOffset_) {}
	};

	EBlockType MutateBlockTop(ChunkPos cp, float x, float y, float z, bool destroy);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APlayerCharacter* playerCharacter;
	ChunkPos curPos = ChunkPos(0, 0);
	bool initChunks = true;

	float delayGenTimer;
	float delayDestroyTimer;
	TMap<ChunkPos, ChunkInfo> allChunkInfo;
	TMap<ChunkPos, int> curMeshes; // Maps chunk pos to procedural mesh section index
	TSet<ChunkPos> meshesToDestroy;
	TArray<FSetElementId> meshesToDestroyIndices;
	TArray<TPair<ChunkPos, FVector>> meshesToGen; // Delays generation of chunks
	TArray<int> availableIndices;

	EBlockType SetBlockType(int x, int y, int z);
	void GenChunk(ChunkPos cp, FVector worldOffset);
	void AddToGenerate(ChunkPos posToAdd);
	void AddToDestroy(ChunkPos posToDestroy);
	void RenderLogic(ChunkPos posDelta);

	static TArray<FVector> Vertices;
	static TArray<int32> Triangles;
	static TArray<FVector2D> UVs;
	void AddUVs(EFaceDir faceDir, EBlockType blockType);
	void AddFace(FVector blockPos, EFaceDir faceDir, EBlockType blockType, int index, FVector worldOffset);
	void GenMesh(ChunkPos cp, bool newGen);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
