// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkGenerator.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class CS378_FINAL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float ExplosionRadius = 1;

	UFUNCTION(BlueprintCallable)
		void MutateBlocks(bool isDestroy);

	int chunkSize;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
