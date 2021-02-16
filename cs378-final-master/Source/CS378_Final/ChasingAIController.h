// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ChasingAIController.generated.h"

/**
 *
 */
UCLASS()
class CS378_FINAL_API AChasingAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
		float CensorRadius = 0.0;
};