// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasingAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void AChasingAIController::BeginPlay() {
	Super::BeginPlay();
}

void AChasingAIController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn->GetDistanceTo(GetPawn()) < 100000.0f) {
		SetFocus(PlayerPawn);
		MoveToLocation(PlayerPawn->GetActorLocation());
		//MoveToActor(PlayerPawn, CensorRadius);
	}
}