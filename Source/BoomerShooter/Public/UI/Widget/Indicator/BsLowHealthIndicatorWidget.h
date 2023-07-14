// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BsLowHealthIndicatorWidget.generated.h"

class UBsHealthComponent;

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsLowHealthIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void BindToHealthComponent(UBsHealthComponent* InHealthComponent);

	UFUNCTION(BlueprintNativeEvent)
	void OwnerLowHealth();

	UFUNCTION(BlueprintCallable)
	bool IsOwnerLowHealth() const;
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TSoftObjectPtr<UBsHealthComponent> HealthComponent;	
};
