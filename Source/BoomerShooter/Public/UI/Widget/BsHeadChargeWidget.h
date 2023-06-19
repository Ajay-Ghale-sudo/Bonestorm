// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BsHeadChargeWidget.generated.h"

class ABsSeveredHeadBase;
/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsHeadChargeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void BindToSeveredHead(ABsSeveredHeadBase* SeveredHead);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateHeadCharge(float HeadCharge);

	UFUNCTION(BlueprintNativeEvent)
	void HideWidget();

	UFUNCTION()
	void RefreshWidget();

protected:

	UPROPERTY()
	ABsSeveredHeadBase* AttachedHead;

};