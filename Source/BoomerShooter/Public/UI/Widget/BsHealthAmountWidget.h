// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "BsHealthAmountWidget.generated.h"


class UBsHealthComponent;

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsHealthAmountWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindToHealthComponent(UBsHealthComponent* InHealthComponent);
	void RefreshHealthAmount();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void UpdateWidget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UBsHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float HealthAmount = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float MaxHealthAmount = 1.f;
	
};
