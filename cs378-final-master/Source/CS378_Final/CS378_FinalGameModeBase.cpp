// Copyright Epic Games, Inc. All Rights Reserved.


#include "CS378_FinalGameModeBase.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"

ACS378_FinalGameModeBase::ACS378_FinalGameModeBase()
{
	PlayerControllerClass = APlayerCharacterController::StaticClass();

	// set default pawn class to our character class
	static ConstructorHelpers::FObjectFinder<UClass> pawnBPClass(TEXT("Class'/Game/Blueprints/PlayerCharacterBP.PlayerCharacterBP_C'"));

	if (pawnBPClass.Object)
	{
		UClass* pawnBP = (UClass*)pawnBPClass.Object;
		DefaultPawnClass = pawnBP;
	}
	else
	{
		DefaultPawnClass = APlayerCharacter::StaticClass();
	}

	// set default controller class to our controller class
	static ConstructorHelpers::FObjectFinder<UClass> controllerBPClass(TEXT("Class'/Game/Blueprints/PlayerCharacterController_BP.PlayerCharacterController_BP_C'"));
	if(controllerBPClass.Object) {
		UClass* controllerBP = (UClass*)controllerBPClass.Object;
		PlayerControllerClass = controllerBP;
	}
	else {
		PlayerControllerClass = APlayerCharacterController::StaticClass();
	}
}


