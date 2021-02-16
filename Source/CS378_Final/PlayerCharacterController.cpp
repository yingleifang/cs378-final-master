// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"
#include "Gun.h"

APlayerCharacterController::APlayerCharacterController()
{

}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacterController::InteractPressed);
	InputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacterController::InteractReleased);
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacterController::JumpPressed);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacterController::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacterController::EndSprint);
	InputComponent->BindAction("Inventory", IE_Released, this, &APlayerCharacterController::ToggleInventory);
	InputComponent->BindAction("ShootDestroy", IE_Pressed, this, &APlayerCharacterController::ShootDestroy);
	InputComponent->BindAction("ShootGenerate", IE_Pressed, this, &APlayerCharacterController::ShootGen);
	// InputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacterController::StartCrouch);
	// InputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacterController::EndCrouch);

	InputComponent->BindAxis("Move_Forward", this, &APlayerCharacterController::MoveForward);
	InputComponent->BindAxis("Move_Right", this, &APlayerCharacterController::MoveRight);
	InputComponent->BindAxis("Turn", this, &APlayerCharacterController::TurnCharacter);
	InputComponent->BindAxis("LookUp", this, &APlayerCharacterController::LookUpCharacter);
}

void APlayerCharacterController::JumpPressed()
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->JumpInit();
	}
}

void APlayerCharacterController::InteractPressed()
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->Interact();
	}
}

void APlayerCharacterController::InteractReleased()
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->InteractFinalize();
	}
}

void APlayerCharacterController::StartSprint()
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->StartSprint();
	}
}

void APlayerCharacterController::EndSprint()
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->EndSprint();
	}
}

void APlayerCharacterController::StartCrouch()
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->StartCrouch();
	}
}

void APlayerCharacterController::EndCrouch()
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->EndCrouch();
	}
}

void APlayerCharacterController::MoveForward(float value)
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->MoveForward(value);
	}
}

void APlayerCharacterController::MoveRight(float value)
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->MoveRight(value);
	}
}

void APlayerCharacterController::TurnCharacter(float value)
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->AddControllerYawInput(value);
	}
}

void APlayerCharacterController::LookUpCharacter(float value)
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->RotateSpringArm(value, 0.f, 0.f);
	}
}

void APlayerCharacterController::ShootDestroy() 
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->Shoot(true);
	}
}

void APlayerCharacterController::ShootGen()
{
	APlayerCharacter* curChar = Cast<APlayerCharacter>(this->GetCharacter());

	if (curChar) {
		curChar->Shoot(false);
	}
}

void APlayerCharacterController::ToggleInventory()
{
	APlayerCharacter* Char = Cast<APlayerCharacter>(GetPawn());
	
	if (InventoryWidgetRef)
	{
		if (bIsInventoryOpen)
		{
			bIsInventoryOpen = false;
			InventoryWidgetRef->RemoveFromViewport();
		}
		else
		{
			bIsInventoryOpen = true;
 
			//Re-populate the ItemsArray
			InventoryWidgetRef->ItemsArray = Char->GetInventory();
			
			InventoryWidgetRef->Show();
		}
		
	}
}

void APlayerCharacterController::OnPossess(APawn* InPawn) {

	Super::OnPossess(InPawn);

	InventoryWidgetRef = CreateWidget<UInventoryWidget>(this, InventoryWidgetBP);
	bIsInventoryOpen = false;

	// if(!InventoryWidgetBP) {
	// 	 GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Cant find BP"));
	// }

	if(InventoryWidgetRef){
		InventoryWidgetRef->RemoveFromViewport();
	} else {
		 GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Cant find widget"));
	}
}

