// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

void UInventorySlotWidget::SetItemCount(int32 Item)
{
	text = FString::FromInt(Item);
}