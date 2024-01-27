// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BoomerShooterGameModeBase.generated.h"

class ABsWeaponBase;
/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABoomerShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	/**
	 * @brief Initializes the Character with defaults.
	 */
	virtual void InitCharacter();

protected:
	/**
	 * @brief The default weapon to have for this GameMode.
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly,  Category = "GameMode|Character")
	TSubclassOf<ABsWeaponBase> DefaultWeaponClass;
};
