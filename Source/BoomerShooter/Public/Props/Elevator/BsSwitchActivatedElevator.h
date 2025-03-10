﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BsElevatorBase.h"
#include "BsSwitchActivatedElevator.generated.h"

class UBsSwitchTriggerComponent;
UCLASS()
class BOOMERSHOOTER_API ABsSwitchActivatedElevator : public ABsElevatorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsSwitchActivatedElevator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief Binds the switch trigger event
	 */
	virtual void BindSwitchTrigger();

	/**
	 * @brief Called when the switch is triggered
	 */
	UFUNCTION()
	void OnSwitchTriggered();

protected:
	/**
	 * @brief The switch trigger component
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Evelvator|Switch")
	UBsSwitchTriggerComponent* SwitchTriggerComponent;
};