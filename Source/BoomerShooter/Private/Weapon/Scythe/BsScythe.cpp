// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Scythe/BsScythe.h"

#include "Weapon/Projectile/BsProjectileBase.h"

void ABsScythe::Fire()
{
	if (!bCanAttack) return;
	Super::Fire();
	
	if (WeaponMode == EScytheWeaponMode::ESWM_Melee)
	{
		MeleeAttack();
	}
	else if (WeaponMode == EScytheWeaponMode::ESWM_Range)
	{
		RangeAttack();
	}
}

void ABsScythe::RangeAttack()
{
	// Spawn Projectile
	UWorld* World = GetWorld();
	if (ProjectileClass && World)
	{
		if (ABsProjectileBase* Projectile = Cast<ABsProjectileBase>(World->SpawnActor(ProjectileClass, &GetActorTransform())))
		{
			
		}
	}
}

void ABsScythe::MeleeAttack()
{
	OnMeleeAttack();
}

void ABsScythe::OnMeleeAttack_Implementation()
{
	
}

void ABsScythe::SetWeaponMode(EScytheWeaponMode NewMode)
{
	WeaponMode = NewMode;
}
