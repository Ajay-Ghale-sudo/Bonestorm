// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "BsDashAmountWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsDashAmountWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetDashAmount(int32 Amount);
};
