// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Enemy/BsFlyingEnemy.h"
#include "Data/AttackResult.h"
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

	bool TargetInLOS(const AActor* Target) const;
	bool TargetInRange(const AActor* Target) const;
	bool LocationInRange(const FVector& Location) const;

	UFUNCTION(BlueprintCallable)
	bool CanAttackTarget(const AActor* Target) const;
	
	/**
	 * @brief Spawns a projectile aimed at the target.
	 * @param Target The target to attack
	 * @returns The result of the attack
	 */
	UFUNCTION(BlueprintCallable)
	EAttackResult RangeAttack(const ACharacter* Target);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dragon")
	TSubclassOf<ABsProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dragon")
	float MaxAttackRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dragon")
	float MinAttackRange = 500.f;
};