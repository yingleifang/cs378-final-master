// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkGenerator.h"
#include "SimplexNoiseBPLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChunkGenerator::AChunkGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChunkMesh = CreateDefaultSubobject<URuntimeMeshComponentStatic>("ChunkMesh");
	ChunkMesh->AttachTo(RootComponent);

	ChunkMeshMat = CreateDefaultSubobject<UMaterial>("ChunkMeshMat");

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/TextMats/MinecraftBlockos_Mat.MinecraftBlockos_Mat'"));
	if (Material.Object) {
		ChunkMeshMat = (UMaterial*)Material.Object;
	}
	else {
		ChunkMeshMat = CreateDefaultSubobject<UMaterial>("ChunkMeshMat");
	}
}

// Called when the game starts or when spawned
void AChunkGenerator::BeginPlay()
{
	Super::BeginPlay();

	playerCharacter = (APlayerCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	initChunks = true;

	for (int i = 0; i < (renderDist * 2 + 7) * (renderDist * 2 + 7); i++) { // Caps available indices at 3 unit larger than render distance square 
		availableIndices.Add(i);
	}

	/*delayGenTimer = renderChunkDelay;
	delayDestroyTimer = destroyChunkDelay;*/
}

EBlockType AChunkGenerator::SetBlockType(int x, int y, int z) {

	if (z < 2) {
		return EBlockType::UNBREAKABLE;
	}

	float landNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(x, y, 2.3f, 0.5f, 4, 0.00005f, true) * 25 + 4;
	float stoneLandNoise = USimplexNoiseBPLibrary::GetSimplexNoise3D_EX(landNoise, y, x, 2.3f, 0.2f, 3, 0.0002f, true);

	float desertNoise = USimplexNoiseBPLibrary::GetSimplexNoise3D_EX(x, y, landNoise, 2.5f, 0.2f, 2, 0.00001f, true);
	// float desertLandNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(x, y, 2.3f, 0.3f, 2, 0.00005f, true) * 25 + 4;

	if (desertNoise > 0.8) {
		if (z > landNoise && z > 3) {
			return EBlockType::AIR;
		}
		else {
			return EBlockType::SAND;
		}
	}

	if (z > landNoise && z > 3) {
		return EBlockType::AIR;
	} 
	else if (stoneLandNoise < 0.25) {
		return EBlockType::STONE;
	}
	else {
		return EBlockType::DIRT;
	}
}

void AChunkGenerator::GenChunk(ChunkPos cp, FVector worldOffset) {
	FVector chunkOffset = FVector(cp.x * blockSize * width, cp.y * blockSize * width, 0.0f);
	ChunkInfo chunkInfo = ChunkInfo(worldOffset);

	for (int x = 0; x < width + 2; x++) {
		int blockX = (x - 1) * blockSize;
		for (int y = 0; y < width + 2; y++) {
			int blockY = (y - 1) * blockSize;
			for (int z = 0; z < height; z++) {
				chunkInfo.data[x][y][z] = SetBlockType(chunkOffset.X + blockX, chunkOffset.Y + blockY, z);
			}
		}
	}

	allChunkInfo.Add(cp, chunkInfo);
}

void AChunkGenerator::AddToGenerate(ChunkPos posToAdd) {
	FVector worldOffset = FVector(posToAdd.x * blockSize * width - (blockSize * width) / 2, posToAdd.y * blockSize * width - (blockSize * width) / 2, 0.0f);
	/*TPair<ChunkPos, FVector> meshToGen;
	meshToGen.Key = posToAdd;
	meshToGen.Value = worldOffset;
	meshesToGen.Add(meshToGen);*/

	GenChunk(posToAdd, worldOffset);
	GenMesh(posToAdd, true);
}

void AChunkGenerator::AddToDestroy(ChunkPos posToDestroy) {
	if (curMeshes.Contains(posToDestroy)) {
		availableIndices.Add(curMeshes[posToDestroy]);
		ChunkMesh->ClearSection(0, curMeshes[posToDestroy]);
		/*meshesToDestroy.Add(posToDestroy);
		meshesToDestroyIndices.Add(meshesToDestroy.FindId(posToDestroy));
		UE_LOG(LogTemp, Log, TEXT("Destroy ID: %d!"), meshesToDestroy.FindId(posToDestroy).AsInteger());*/
	}
	else
		UE_LOG(LogTemp, Log, TEXT("Attempting to destroy unavailable mesh at %d,%d!"), posToDestroy.x, posToDestroy.y);
}

void AChunkGenerator::RenderLogic(ChunkPos posDelta) {
	while (posDelta.x < 0) {
		for (int yOffset = -renderDist; yOffset <= renderDist; yOffset++) {
			ChunkPos genOffset = curPos - ChunkPos(renderDist, yOffset);
			ChunkPos destroyOffset = curPos + ChunkPos(renderDist - posDelta.x, yOffset);
			AddToGenerate(genOffset);
			AddToDestroy(destroyOffset);
		}
		posDelta.x += 1;
	}

	while (posDelta.x > 0) {
		for (int yOffset = -renderDist; yOffset <= renderDist; yOffset++) {
			ChunkPos genOffset = curPos + ChunkPos(renderDist, yOffset);
			ChunkPos destroyOffset = curPos - ChunkPos(renderDist + posDelta.x, yOffset);
			AddToGenerate(genOffset);
			AddToDestroy(destroyOffset);
		}
		posDelta.x -= 1;
	}

	while (posDelta.y < 0) {
		for (int xOffset = -renderDist; xOffset <= renderDist; xOffset++) {
			ChunkPos genOffset = curPos - ChunkPos(xOffset, renderDist);
			ChunkPos destroyOffset = curPos + ChunkPos(xOffset, renderDist - posDelta.y);
			AddToGenerate(genOffset);
			AddToDestroy(destroyOffset);
		}
		posDelta.y += 1;
	}

	while (posDelta.y > 0) {
		for (int xOffset = -renderDist; xOffset <= renderDist; xOffset++) {
			ChunkPos genOffset = curPos + ChunkPos(xOffset, renderDist);
			ChunkPos destroyOffset = curPos - ChunkPos(xOffset, renderDist + posDelta.y);
			AddToGenerate(genOffset);
			AddToDestroy(destroyOffset);
		}
		posDelta.y -= 1;
	}
}

// Called every frame
void AChunkGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChunkPos lastPos = curPos;
	curPos = ChunkPos((int)(playerCharacter->GetActorLocation().X / blockSize) / width, (int)(playerCharacter->GetActorLocation().Y / blockSize) / width);
	ChunkPos posDelta = curPos - lastPos; // Position delta

	// Logging stuff
	// UE_LOG(LogTemp, Log, TEXT("curPos: %d,%d"), curPos.x, curPos.y);
	// UE_LOG(LogTemp, Log, TEXT("posDelta: %d,%d"), posDelta.x, posDelta.y);

	TArray<ChunkPos> toRemove;

	// If position changed, remove chunks outside and add chunks inside render range
	if (curPos.x != lastPos.x || curPos.y != lastPos.y) {
		RenderLogic(posDelta);
	}

	if (initChunks) {
		initChunks = false;

		// Generate all chunks within render distance (square)
		for (int xOffset = -renderDist; xOffset <= renderDist; xOffset++) {
			for (int yOffset = -renderDist; yOffset <= renderDist; yOffset++) {
				ChunkPos curOffset = curPos + ChunkPos(xOffset, yOffset);
				FVector worldOffset = FVector(curOffset.x * blockSize * width - (blockSize * width) / 2, curOffset.y * blockSize * width - (blockSize * width) / 2, 0.0f);
				UE_LOG(LogTemp, Log, TEXT("genChunks At: %d,%d"), curOffset.x, curOffset.y);
				GenChunk(curOffset, worldOffset);
				GenMesh(curOffset, true);
			}
		}
	}

	// Chunk generator delayer
	//if (meshesToGen.Num() > 0 && delayGenTimer < 0.0f) {
	//	UE_LOG(LogTemp, Log, TEXT("meshesToGen At"));

	//	delayGenTimer = renderChunkDelay;
	//	TPair<ChunkPos, FVector> meshToGen;
	//	meshToGen = meshesToGen.Pop();

	//	// If to generated chunk is in to destroy, remove from to destroy
	//	if (meshesToDestroy.Contains(meshToGen.Key)) {
	//		meshesToDestroy.Remove(meshToGen.Key);
	//	}

	//	if (!allChunkInfo.Contains(meshToGen.Key)) {
	//		UE_LOG(LogTemp, Log, TEXT("genChunks At: %d,%d"), meshToGen.Key.x, meshToGen.Key.y);
	//		GenChunk(meshToGen.Key, meshToGen.Value);
	//		GenMesh(meshToGen.Key);
	//	}
	//	else if (allChunkInfo.Contains(meshToGen.Key) && !curMeshes.Contains(meshToGen.Key)) {
	//		GenMesh(meshToGen.Key);
	//	}
	//}
	//else {
	//	delayGenTimer -= DeltaTime;
	//}

	//// Chunk destroyer delayer
	//if (meshesToDestroy.Num() > 0 && delayDestroyTimer < 0.0f) {
	//	delayDestroyTimer = destroyChunkDelay;
	//	FSetElementId meshToDestroyIndex = meshesToDestroyIndices.Pop();
	//	ChunkPos meshIndexToDestroy = meshesToDestroy[meshToDestroyIndex];
	//	meshesToDestroy.Remove(meshIndexToDestroy);
	//	UE_LOG(LogTemp, Log, TEXT("destroyChunks At: %d,%d"), meshIndexToDestroy.x, meshIndexToDestroy.y);
	//	availableIndices.Add(curMeshes[meshIndexToDestroy]);
	//	ChunkMesh->ClearSection(0, curMeshes[meshIndexToDestroy]);
	//	//ChunkMesh->ClearMeshSection(curMeshes[meshIndexToDestroy]);
	//}
	//else {
	//	delayDestroyTimer -= DeltaTime;
	//}
}


void AChunkGenerator::AddUVs(EFaceDir faceDir, EBlockType blockType) {
	float uvOffsetX = 0.f;
	float uvOffsetY = 3.f;

	switch (blockType) {
	case EBlockType::DIRT:
		if (faceDir == EFaceDir::UP) {
			uvOffsetX = 0.f;
			uvOffsetY = 2.f;
		}
		break;

	case EBlockType::STONE:
		uvOffsetX = 1.f;
		uvOffsetY = 3.f;
		break;

	case EBlockType::SAND:
		uvOffsetX = 2.f;
		uvOffsetY = 3.f;
		break;

	case EBlockType::UNBREAKABLE:
		uvOffsetX = 3.f;
		uvOffsetY = 3.f;
		break;
	}

	switch (faceDir) {
	case EFaceDir::UP:
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		break;

	case EFaceDir::DOWN:
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		break;

	case EFaceDir::LEFT:
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		break;

	case EFaceDir::RIGHT:
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		break;

	case EFaceDir::FRONT:
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		break;

	case EFaceDir::BACK:
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((1.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (1.f + uvOffsetY) / uvMod));
		UVs.Add(FVector2D((0.f + uvOffsetX) / uvMod, (0.f + uvOffsetY) / uvMod));
		break;
	}

}

TArray<FVector> AChunkGenerator::Vertices;
TArray<int32> AChunkGenerator::Triangles;
TArray<FVector2D> AChunkGenerator::UVs;

void AChunkGenerator::AddFace(FVector blockPos, EFaceDir faceDir, EBlockType blockType, int index, FVector worldOffset) {
	switch (faceDir) {
	case EFaceDir::UP:
		Vertices.Add((blockPos + FVector(0.0f, 0.0f, 1.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(0.0f, 1.0f, 1.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(1.0f, 1.0f, 1.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(1.0f, 0.0f, 1.0f)) * blockSize + worldOffset);
		break;

	case EFaceDir::DOWN:
		Vertices.Add((blockPos + FVector(1.0f, 0.0f, 0.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(1.0f, 1.0f, 0.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(0.0f, 1.0f, 0.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(0.0f, 0.0f, 0.0f)) * blockSize + worldOffset);
		break;

	case EFaceDir::LEFT:
		Vertices.Add((blockPos + FVector(0.0f, 0.0f, 0.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(0.0f, 1.0f, 0.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(0.0f, 1.0f, 1.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(0.0f, 0.0f, 1.0f)) * blockSize + worldOffset);
		break;

	case EFaceDir::RIGHT:
		Vertices.Add((blockPos + FVector(1.0f, 0.0f, 1.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(1.0f, 1.0f, 1.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(1.0f, 1.0f, 0.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(1.0f, 0.0f, 0.0f)) * blockSize + worldOffset);
		break;

	case EFaceDir::FRONT:
		Vertices.Add((blockPos + FVector(0.0f, 0.0f, 0.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(0.0f, 0.0f, 1.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(1.0f, 0.0f, 1.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(1.0f, 0.0f, 0.0f)) * blockSize + worldOffset);
		break;

	case EFaceDir::BACK:
		Vertices.Add((blockPos + FVector(0.0f, 1.0f, 1.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(0.0f, 1.0f, 0.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(1.0f, 1.0f, 0.0f)) * blockSize + worldOffset);
		Vertices.Add((blockPos + FVector(1.0f, 1.0f, 1.0f)) * blockSize + worldOffset);
		break;
	}

	Triangles.Add(0 + index * 4);
	Triangles.Add(1 + index * 4);
	Triangles.Add(2 + index * 4);
	Triangles.Add(3 + index * 4);
	Triangles.Add(0 + index * 4);
	Triangles.Add(2 + index * 4);

	AddUVs(faceDir, blockType);
}

void AChunkGenerator::GenMesh(ChunkPos cp, bool newGen) {

	if (availableIndices.Num() == 0) {
		UE_LOG(LogTemp, Log, TEXT("No available indices! (Should not happen)"));
		return;
	}
	if (newGen) {
		curMeshes.Add(cp, availableIndices.Pop());
	}
	Vertices.Empty();
	Triangles.Empty();
	UVs.Empty();
	int index = 0;

	for (int x = 1; x <= width; x++) {
		for (int y = 1; y <= width; y++) {
			for (int z = 0; z < height; z++) {

				// If not air and surrounded by air, gen block face
				if (allChunkInfo[cp].data[x][y][z] != EBlockType::AIR) {
					FVector blockPos = FVector(x - 1, y - 1, z);

					if (z + 1 < height && allChunkInfo[cp].data[x][y][z + 1] == EBlockType::AIR)
						AddFace(blockPos, EFaceDir::UP, allChunkInfo[cp].data[x][y][z], index++, allChunkInfo[cp].worldOffset);
					if (z - 1 >= 0 && allChunkInfo[cp].data[x][y][z - 1] == EBlockType::AIR)
						AddFace(blockPos, EFaceDir::DOWN, allChunkInfo[cp].data[x][y][z], index++, allChunkInfo[cp].worldOffset);
					if (allChunkInfo[cp].data[x - 1][y][z] == EBlockType::AIR)
						AddFace(blockPos, EFaceDir::LEFT, allChunkInfo[cp].data[x][y][z], index++, allChunkInfo[cp].worldOffset);
					if (allChunkInfo[cp].data[x + 1][y][z] == EBlockType::AIR)
						AddFace(blockPos, EFaceDir::RIGHT, allChunkInfo[cp].data[x][y][z], index++, allChunkInfo[cp].worldOffset);
					if (allChunkInfo[cp].data[x][y - 1][z] == EBlockType::AIR)
						AddFace(blockPos, EFaceDir::FRONT, allChunkInfo[cp].data[x][y][z], index++, allChunkInfo[cp].worldOffset);
					if (allChunkInfo[cp].data[x][y + 1][z] == EBlockType::AIR)
						AddFace(blockPos, EFaceDir::BACK, allChunkInfo[cp].data[x][y][z], index++, allChunkInfo[cp].worldOffset);
				}
			}
		}
	}

	ChunkMesh->CreateSectionFromComponents(0, curMeshes[cp], 0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FRuntimeMeshTangent>());
	ChunkMesh->SetMaterial(0, ChunkMeshMat);
	//ChunkMesh->CreateMeshSection(curMeshes[cp], Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

EBlockType AChunkGenerator::MutateBlockTop(ChunkPos cp, float x, float y, float z, bool destroy) {

	UE_LOG(LogTemp, Log, TEXT("XYZ mutates: %f,%f,%f"), x, y, z);

	if (!allChunkInfo.Contains(cp)) {
		UE_LOG(LogTemp, Log, TEXT("invalid CP: %d,%d"), cp.x, cp.y);
		return EBlockType::UNBREAKABLE;
	}
	UE_LOG(LogTemp, Log, TEXT("WOff mutates: %f,%f,%f"), allChunkInfo[cp].worldOffset.X, allChunkInfo[cp].worldOffset.Y, allChunkInfo[cp].worldOffset.Z);

	int blockX = (x - allChunkInfo[cp].worldOffset.X) / blockSize;
	int blockY = (y - allChunkInfo[cp].worldOffset.Y) / blockSize;
	int blockZ = (z - allChunkInfo[cp].worldOffset.Z) / blockSize;

	if (blockX < 0 || blockX >= width || blockY < 0 || blockY >= width || blockZ < 0 || blockZ >= height) {
		UE_LOG(LogTemp, Log, TEXT("invalid blocks: %d,%d,%d"), blockX, blockY, blockZ - 1);
		return EBlockType::UNBREAKABLE;
	}

	if (destroy) {
		if (blockZ - 1 <= 0) {
			UE_LOG(LogTemp, Log, TEXT("invalid blocks: %d,%d,%d"), blockX, blockY, blockZ - 1);
			return EBlockType::UNBREAKABLE;
		}

		UE_LOG(LogTemp, Log, TEXT("destroyBlockAt CP: %d,%d"), cp.x, cp.y);
		UE_LOG(LogTemp, Log, TEXT("destroyBlockAt: %d,%d,%d"), blockX, blockY, blockZ - 1);
		allChunkInfo[cp].data[blockX][blockY][blockZ - 1] = EBlockType::AIR;
		GenMesh(cp, false);
		return allChunkInfo[cp].data[blockX][blockY][blockZ - 1];
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("createBlockAt CP: %d,%d"), cp.x, cp.y);
		UE_LOG(LogTemp, Log, TEXT("createBlockAt: %d,%d,%d"), blockX, blockY, blockZ);
		allChunkInfo[cp].data[blockX][blockY][blockZ] = EBlockType::DIRT;
		GenMesh(cp, false);
		return allChunkInfo[cp].data[blockX][blockY][blockZ];
	}
}