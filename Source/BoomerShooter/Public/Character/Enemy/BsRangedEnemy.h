
#pragma once

#include "Character/Enemy/BsEnemyBase.h"
#include "BsRangedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsRangedEnemy : public ABsEnemyBase
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged Enemy")
	TSubclassOf<ABsProjectileBase> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged Enemy")
	float MaxAttackRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged Enemy")
	float MinAttackRange = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged Enemy")
	float AttackRate = 1.0f;

	/**
	 * @brief The accuracy of the enemy when aiming at the target.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged Enemy")
	float Accuracy = 0.9f;

	/**
	 * @brief The maximum deviation from the target's location when aiming.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged Enemy")
	float AccuracyDeviation = 250.0f;
};