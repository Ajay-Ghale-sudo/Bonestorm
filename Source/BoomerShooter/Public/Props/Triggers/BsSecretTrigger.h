// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BsBaseTrigger.h"
#include "GameplayTagContainer.h"
#include "BsSecretTrigger.generated.h"

UCLASS()
class BOOMERSHOOTER_API ABsSecretTrigger : public ABsBaseTrigger
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsSecretTrigger();

protected:
	virtual void StartTrigger(AActor* TriggeredActor) override;

protected:
	/**
	 * @brief GameplayTag to send to the UI Subsystem when the secret is triggered.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers|Secret")
	FGameplayTag SecretTag;
};
