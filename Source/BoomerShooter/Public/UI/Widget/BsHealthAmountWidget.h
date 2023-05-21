// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BsHealthAmountWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsHealthAmountWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetHealthAmount(float Value);
	
};
