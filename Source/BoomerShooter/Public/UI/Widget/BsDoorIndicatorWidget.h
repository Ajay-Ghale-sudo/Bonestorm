// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "BsDoorIndicatorWidget.generated.h"

class ABsDoorBase;
UCLASS()
class BOOMERSHOOTER_API UBsDoorIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()
	friend class ABsDoorBase;

protected:
	void SetDoorIndicator(ABsDoorBase* InDoor);

	UFUNCTION()
	void DoorUpdated();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	ABsDoorBase* Door;
	
};