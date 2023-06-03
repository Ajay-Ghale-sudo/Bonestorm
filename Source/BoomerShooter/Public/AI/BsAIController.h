// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BsAIController.generated.h"


UENUM(BlueprintType)
enum class EBsAIState : uint8
{
	EAIS_Idle UMETA(DisplayName = "Idle"),
	EAIS_Chase UMETA(DisplayName = "Chase"),
	EAIS_Attack UMETA(DisplayName = "Attack"),
	EAIS_Dead UMETA(DisplayName = "Dead"),

	Default_Max UMETA(DisplayName = "Max")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsAIControllerEvent);

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsAIController : public AAIController
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	virtual void SetCurrentState(const EBsAIState NewState);

	virtual void ProcessStateChange();
	
	UFUNCTION()
	virtual void Die();
	virtual void OnDeath();
	
public:
	UPROPERTY(BlueprintAssignable)
	FBsAIControllerEvent OnStateChanged;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	EBsAIState CurrentState = EBsAIState::EAIS_Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EBsAIState PreviousState = EBsAIState::EAIS_Idle;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EBsAIState GetCurrentState() const { return CurrentState; }
};