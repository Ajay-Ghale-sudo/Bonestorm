// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "UObject/Object.h"
#include "BsAISubsystem.generated.h"

class UBsAIDirector;
/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsAISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitAIDirector();

	void OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBsAIDirector* AIDirector;

public:
	UFUNCTION(BlueprintCallable, Category = "AI")
	FORCEINLINE UBsAIDirector* GetAIDirector() const { return AIDirector; }
};