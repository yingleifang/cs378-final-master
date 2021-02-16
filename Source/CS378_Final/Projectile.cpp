// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	chunkSize = AChunkGenerator::height * AChunkGenerator::width;
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::MutateBlocks(bool isDestroy) {
	FVector offset = FVector(0, 400, -500);
	FVector rootLoc = RootComponent->GetComponentLocation() + offset;
	FVector loc = (RootComponent->GetComponentLocation() + (chunkSize / 2)) / chunkSize;
	AChunkGenerator::ChunkPos curPos = AChunkGenerator::ChunkPos((int)loc.X, (int)loc.Y);
	AChunkGenerator* chunkGenerator = (AChunkGenerator*)UGameplayStatics::GetActorOfClass(GetWorld(), AChunkGenerator::StaticClass());
	if (chunkGenerator) {
		chunkGenerator->MutateBlockTop(curPos, rootLoc.X, rootLoc.Y, rootLoc.Z, isDestroy);
	}
}