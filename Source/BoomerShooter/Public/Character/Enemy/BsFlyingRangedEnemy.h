// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BsRangedEnemy.h"
#include "BsFlyingRangedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsFlyingRangedEnemy : public ABsRangedEnemy
{
	GENERATED_BODY()

public:
	ABsFlyingRangedEnemy();

	virtual bool LocationInRange(const FVector& Location) const override;
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flying Ranged Enemy")
	float MaxAttackZRange = 500.0f;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxAttackZRange() const { return MaxAttackZRange; }
};