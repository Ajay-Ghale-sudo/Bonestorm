// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BsEnemyBase.h"
#include "BsSkeleton.generated.h"

class UAnimMontage;

UCLASS()
class BOOMERSHOOTER_API ABsSkeleton : public ABsEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABsSkeleton();

	UFUNCTION(BlueprintCallable)
	void MeleeAttack();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Die() override;
	virtual void Attack() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skeleton")
	UAnimMontage* MeleeAttackMontage;
};