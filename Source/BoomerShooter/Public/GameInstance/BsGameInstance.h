// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BsGameInstance.generated.h"

class UBsImpactFxManager;

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact Fx")
	TSubclassOf<UBsImpactFxManager> ImpactFxManagerClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impact Fx")
	UBsImpactFxManager* ImpactFxManager;
};