//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "Chunk.h"
//
//// Sets default values
//AChunk::AChunk()
//{
//	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//	ChunkMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ChunkMesh");
//	SetRootComponent(ChunkMesh);
//	ChunkMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//
//}
//
//// Called when the game starts or when spawned
//void AChunk::BeginPlay()
//{
//	Super::BeginPlay();
//
//}
//
//// Called every frame
//void AChunk::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
//void AChunk::SetRenderDist(float renderDist) {
//	availableIndices.Empty();
//	int curIndex = 0;
//	float renderDistSq = renderDist * renderDist;
//	for (int xOffset = -renderDist; xOffset <= renderDist; xOffset++) {
//		for (int yOffset = -renderDist; yOffset <= renderDist; yOffset++) {
//			if (xOffset * xOffset + yOffset * yOffset <= renderDistSq) {
//				availableIndices.Add(curIndex++);
//			}
//		}
//	}
//}
//
//TArray<FVector> AChunk::Vertices;
//TArray<int32> AChunk::Triangles;
//TArray<FVector2D> AChunk::UVs;
//
//void AChunk::AddFace(FVector blockPos, EFaceDir faceDir, EBlockType blockType, int index) {
//	switch (faceDir) {
//		case EFaceDir::UP:
//			Vertices.Add((blockPos + FVector(0.0f, 0.0f, 1.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(0.0f, 1.0f, 1.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(1.0f, 1.0f, 1.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(1.0f, 0.0f, 1.0f)) * blockSize);
//			break;
//
//		case EFaceDir::DOWN:
//			Vertices.Add((blockPos + FVector(1.0f, 0.0f, 0.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(1.0f, 1.0f, 0.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(0.0f, 1.0f, 0.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(0.0f, 0.0f, 0.0f)) * blockSize);
//			break;
//
//		case EFaceDir::LEFT:
//			Vertices.Add((blockPos + FVector(0.0f, 0.0f, 0.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(0.0f, 1.0f, 0.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(0.0f, 1.0f, 1.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(0.0f, 0.0f, 1.0f)) * blockSize);
//			break;
//
//		case EFaceDir::RIGHT:
//			Vertices.Add((blockPos + FVector(1.0f, 0.0f, 1.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(1.0f, 1.0f, 1.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(1.0f, 1.0f, 0.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(1.0f, 0.0f, 0.0f)) * blockSize);
//			break;
//
//		case EFaceDir::FRONT:
//			Vertices.Add((blockPos + FVector(0.0f, 0.0f, 0.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(0.0f, 0.0f, 1.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(1.0f, 0.0f, 1.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(1.0f, 0.0f, 0.0f)) * blockSize);
//			break;
//
//		case EFaceDir::BACK:
//			Vertices.Add((blockPos + FVector(0.0f, 1.0f, 1.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(0.0f, 1.0f, 0.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(1.0f, 1.0f, 0.0f)) * blockSize);
//			Vertices.Add((blockPos + FVector(1.0f, 1.0f, 1.0f)) * blockSize);
//			break;
//	}
//
//	Triangles.Add(0 + index * 4);
//	Triangles.Add(1 + index * 4);
//	Triangles.Add(2 + index * 4);
//	Triangles.Add(3 + index * 4);
//	Triangles.Add(0 + index * 4);
//	Triangles.Add(2 + index * 4);
//
//	UVs.Add(FVector2D(0.0f, 0.0f));
//	UVs.Add(FVector2D(1.0f, 0.0f));
//	UVs.Add(FVector2D(1.0f, 1.0f));
//	UVs.Add(FVector2D(0.0f, 1.0f));
//}
//
//void AChunk::GenMesh(AChunkGenerator::ChunkPos cp) {
//
//	if (curMeshes.Contains(cp)) {
//		ChunkMesh->ClearMeshSection(curMeshes[cp]);
//	}
//	else {
//		if (availableIndices.Num() == 0) return;
//		curMeshes.Add(cp, availableIndices.Pop());
//	}
//	Vertices.Empty();
//	Triangles.Empty();
//	UVs.Empty();
//	int index = 0;
//
//	for (int x = 1; x <= width; x++) {
//		for (int y = 1; y <= width; y++) {
//			for (int z = 0; z < height; z++) {
//
//				// If not air and surrounded by air, gen block face
//				if (allChunkInfo[cp][x][y][z] != EBlockType::AIR) {
//					FVector blockPos = FVector(x - 1, y - 1, z);
//
//					if (z + 1 < height && allChunkInfo[cp][x][y][z + 1] == EBlockType::AIR)
//						AddFace(blockPos, EFaceDir::UP, allChunkInfo[cp][x][y][z], index++);
//					if (z - 1 >= 0 && allChunkInfo[cp][x][y][z - 1] == EBlockType::AIR)
//						AddFace(blockPos, EFaceDir::DOWN, allChunkInfo[cp][x][y][z], index++);
//					if (allChunkInfo[cp][x - 1][y][z] == EBlockType::AIR)
//						AddFace(blockPos, EFaceDir::LEFT, allChunkInfo[cp][x][y][z], index++);
//					if (allChunkInfo[cp][x + 1][y][z] == EBlockType::AIR)
//						AddFace(blockPos, EFaceDir::RIGHT, allChunkInfo[cp][x][y][z], index++);
//					if (allChunkInfo[cp][x][y - 1][z] == EBlockType::AIR)
//						AddFace(blockPos, EFaceDir::FRONT, allChunkInfo[cp][x][y][z], index++);
//					if (allChunkInfo[cp][x][y + 1][z] == EBlockType::AIR)
//						AddFace(blockPos, EFaceDir::BACK, allChunkInfo[cp][x][y][z], index++);
//				}
//			}
//		}
//	}
//
//	ChunkMesh->CreateMeshSection_LinearColor(curMeshes[cp], Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
//}