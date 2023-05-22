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
	void SetDashAmount(float Amount);

	UFUNCTION(BlueprintNativeEvent)
	void SetDashEnabled(bool bDashEnabled);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DashWidget|Color")
	FLinearColor EnabledColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DashWidget|Color")
	FLinearColor DisabledColor;
	
};
