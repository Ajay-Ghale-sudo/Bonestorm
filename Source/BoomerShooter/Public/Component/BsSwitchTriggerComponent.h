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

	UFUNCTION(BlueprintCallable)
	void CheckSwitches();

public:
	FBsSwitchTriggerEvent OnTriggered;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSwitchToggled();

	UFUNCTION(BlueprintNativeEvent)
	void Triggered();

	virtual void TriggeredInternal();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switch Trigger")
	TArray<TSoftObjectPtr<AActor>> SwitchActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Switch Trigger")
	TArray<class UBsToggleSwitchComponent*> Switches;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switch Trigger")
	bool bTriggerOnce = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switch Trigger")
	bool bTriggered = false;
	
public:
};
