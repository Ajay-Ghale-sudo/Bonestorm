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


	UFUNCTION(BlueprintCallable)
	void Toggle();

	UFUNCTION(BlueprintCallable)
	void SetIsOn(bool bNewOn = true);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBsToggleSwitchEvent OnToggle;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Toggle Switch")
	bool bIsOn = false;
	
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsOn() const { return bIsOn; }
};