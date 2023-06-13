// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BsToggleSwitchComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsToggleSwitchEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsToggleSwitchComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBsToggleSwitchComponent();

	/**
	 * @brief Toggles the switch state
	 */
	UFUNCTION(BlueprintCallable)
	void Toggle();

	/**
	 * @brief Sets the switch state
	 * @param bNewOn The new switch state
	 */
	UFUNCTION(BlueprintCallable)
	void SetIsOn(bool bNewOn = true);

public:
	/**
	 * @brief Called when the switch is toggled
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBsToggleSwitchEvent OnToggle;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	/**
	 * @brief The current switch state
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Toggle Switch")
	bool bIsOn = false;
	
public:
	/**
	 * @brief Returns the current switch state 
	 * @return The current switch state
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsOn() const { return bIsOn; }
};