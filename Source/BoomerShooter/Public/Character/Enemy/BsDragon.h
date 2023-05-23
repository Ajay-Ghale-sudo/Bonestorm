// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Enemy/BsFlyingEnemy.h"
#include "BsDragon.generated.h"


class ABsProjectileBase;

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsDragon : public ABsFlyingEnemy
{
	GENERATED_BODY()


public:
	/**
	 * @brief Spawns a projectile aimed at the target.
	 * @param TargetTransform The Transform of the target to attack
	 */
	UFUNCTION(BlueprintCallable)
	void RangeAttack(const ACharacter* Target);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dragon")
	TSubclassOf<ABsProjectileBase> ProjectileClass;

	
	
};
