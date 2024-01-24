// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BsUISubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FBsUISubsystemDelegate, const FGameplayTag&);

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * @brief UI Event triggered with a GameplayTag Message.
	 */
	FBsUISubsystemDelegate OnUIEventTriggered;

	/**
	 * @brief UI Event Ended for given GameplayTag Message.
	 */
	FBsUISubsystemDelegate OnUIEventEnded;	
};
