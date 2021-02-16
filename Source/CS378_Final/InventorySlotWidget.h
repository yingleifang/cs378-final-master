// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class CS378_FINAL_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	/*Holds a reference to the item texture*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTexture2D* ItemTexture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString text = FString(TEXT("0"));

public:

	UFUNCTION(BlueprintCallable, Category = UI)
	void SetItemCount(int32 itemCount);
};
