// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BsSwitchTriggerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsSwitchTriggerEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsSwitchTriggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBsSwitchTriggerComponent();

	/**
	 * @brief Checks if all switches are on. If so, triggers the event.
	 */
	UFUNCTION(BlueprintCallable)
	void CheckSwitches();

public:
	/**
	 * @brief Called when all switches are on
	 */
	UPROPERTY(BlueprintAssignable)
	FBsSwitchTriggerEvent OnSwitchTriggered;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * @brief Called when a switch is toggled.
	 */
	UFUNCTION()
	void OnSwitchToggled();

	/**
	 * @brief Called when all switches are on.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Triggered();

	/**
	 * @brief Called when all switches are on.
	 */
	virtual void TriggeredInternal();
	
protected:
	/**
	 * @brief Actors that have a UBsToggleSwitchComponent.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switch Trigger")
	TArray<TSoftObjectPtr<AActor>> SwitchActors;

	/**
	 * @brief Switches to watch toggle state.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Switch Trigger")
	TArray<class UBsToggleSwitchComponent*> Switches;

	/**
	 * @brief If true, the trigger will only be triggered once.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switch Trigger")
	bool bTriggerOnce = false;

	/**
	 * @brief If true, the trigger has been triggered.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switch Trigger")
	bool bTriggered = false;
};