// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryWidget.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class CS378_FINAL_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

private:
	/*InventoryWidget reference*/
	UInventoryWidget* InventoryWidgetRef;
 
	/*True if the inventory is currently open - false otherwise*/
	bool bIsInventoryOpen;

public:
	APlayerCharacterController();

	virtual void OnPossess(APawn* InPawn) override;

protected:
	virtual void SetupInputComponent() override;

	/*InventoryWidget Blueprint reference*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetBP;

private:
	UFUNCTION()
		void MoveForward(float value);
	UFUNCTION()
		void MoveRight(float value);
	UFUNCTION()
		void TurnCharacter(float value);
	UFUNCTION()
		void LookUpCharacter(float value);
	UFUNCTION()
		void InteractPressed();
	UFUNCTION()
		void InteractReleased();
	UFUNCTION()
		void ShootDestroy();
	UFUNCTION()
		void ShootGen();
	UFUNCTION()
		void JumpPressed();
	UFUNCTION()
		void StartSprint();
	UFUNCTION()
		void EndSprint();
	UFUNCTION()
		void StartCrouch();
	UFUNCTION()
		void EndCrouch();
	UFUNCTION()
		void ToggleInventory();

};
