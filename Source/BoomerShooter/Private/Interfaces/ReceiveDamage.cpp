// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/ReceiveDamage.h"

#include "Weapon/BsWeaponBase.h"

// Add default functionality here for any IReceiveDamage functions that are not pure virtual.
void IReceiveDamage::ReceiveProjectileDamage(const FHitResult& HitResult, ABsProjectileBase* Projectile,
	const float Damage)
{
}

void IReceiveDamage::ReceiveMeleeDamage(const FHitResult& HitResult, ABsWeaponBase* AttackingWeapon, const float Damage)
{
}
