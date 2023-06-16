// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widget/BsDoorIndicatorWidget.h"
#include "Props/Door/BsDoorBase.h"

void UBsDoorIndicatorWidget::SetDoorIndicator(ABsDoorBase* InDoor)
{
	Door = InDoor;
	if (Door)
	{
		Door->OnDoorStateChanged.AddDynamic(this, &UBsDoorIndicatorWidget::DoorUpdated);
	}
}

void UBsDoorIndicatorWidget::DoorUpdated()
{
	if (Door)
	{
		SetVisibility(Door->IsLocked() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}