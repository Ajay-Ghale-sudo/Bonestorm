// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "ReceiveDamage.generated.h"

class ABsWeaponBase;
class ABsProjectileBase;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UReceiveDamage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BOOMERSHOOTER_API IReceiveDamage
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual void ReceiveProjectileDamage(const FHitResult& HitResult, ABsProjectileBase* Projectile, const float Damage);
	UFUNCTION(BlueprintCallable)
	virtual void ReceiveMeleeDamage(const FHitResult& HitResult, ABsWeaponBase* AttackingWeapon, const float Damage);
};
