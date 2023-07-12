// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/StatTracker/BsGameStatTracker.h"

#include "Character/BsCharacter.h"
#include "Character/Enemy/BsEnemyBase.h"
#include "Component/BsHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/BsWeaponBase.h"
#include "Weapon/Scythe/BsScythe.h"

void UBsGameStatTracker::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UBsGameStatTracker::IncrementStat(EBsPlayerStats Stat)
{
	if (Stat == Unknown || Stat == Max)
	{
		return;
	}
	
	PlayerStats.Stats.FindOrAdd(Stat) += 1.f;
}

void UBsGameStatTracker::AddToStat(EBsPlayerStats Stat, float Amount)
{
	PlayerStats.Stats.FindOrAdd(Stat) += Amount;
}

void UBsGameStatTracker::BindToWeapon(ABsWeaponBase* Weapon)
{
	for (auto& Handle : WeaponHandles)
	{
		Handle.Reset();
	}
	WeaponHandles.Empty();
	
	if (Weapon)
	{
		WeaponHandles.Add(Weapon->OnWeaponFire.AddUFunction(this, IncrementStatFunctionName, ShotsFired));
		WeaponHandles.Add(Weapon->OnWeaponMeleeAttack.AddUFunction(this, IncrementStatFunctionName, MeleeSwings));
		WeaponHandles.Add(Weapon->OnWeaponHeadAttached.AddLambda([this](ABsSeveredHeadBase* Head)
		{
			if (Head)
			{
				IncrementStat(HeadsAttached);
			}
		}));
		WeaponHandles.Add(Weapon->OnWeaponHeadConsumed.AddUFunction(this, IncrementStatFunctionName, HeadsConsumed));

		if (ABsScythe* Scythe = Cast<ABsScythe>(Weapon))
		{
			WeaponHandles.Add(Scythe->OnCreateProjectileEvent.AddUObject(this, &UBsGameStatTracker::BindToProjectile));
		}
	}
}

void UBsGameStatTracker::BindToProjectile(ABsProjectileBase* Projectile)
{
	if (Projectile)
	{
		Projectile->OnDamagedActor.AddUObject(this, &UBsGameStatTracker::OnProjectileDamagedActor);
	}
}

void UBsGameStatTracker::OnProjectileDamagedActor(AActor* Actor)
{
	if (ABsEnemyBase* Enemy = Cast<ABsEnemyBase>(Actor))
	{
		IncrementStat(RangeShotsHit);
	}
}

void UBsGameStatTracker::StartTrackingPlayer()
{
	if (!PlayerCharacter.IsValid())
	{
		PlayerCharacter = Cast<ABsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!PlayerCharacter->IsValidLowLevel())
		{
			return;
		}
	}

	CharacterHandles.Add(PlayerCharacter->OnDash.AddUFunction(this, IncrementStatFunctionName, Dashes));
	CharacterHandles.Add(PlayerCharacter->OnSlideStart.AddUFunction(this, IncrementStatFunctionName, Slides));
	CharacterHandles.Add(PlayerCharacter->OnWeaponChanged.AddUObject(this, &UBsGameStatTracker::BindToWeapon));
	CharacterHandles.Add(PlayerCharacter->OnJump.AddUFunction(this, IncrementStatFunctionName, Jumps));
	CharacterHandles.Add(PlayerCharacter->OnSlideJump.AddUFunction(this, IncrementStatFunctionName, SlideJumps));
	CharacterHandles.Add(PlayerCharacter->OnDashJump.AddUFunction(this, IncrementStatFunctionName, DashJumps));

	if (UBsHealthComponent* HealthComponent = PlayerCharacter->GetHealthComponent())
	{
		HealthComponent->OnDeath.AddDynamic(this, &UBsGameStatTracker::IncrementDeaths);
		DynamicHandles.Add(HealthComponent->OnDeath);
		CharacterHandles.Add(HealthComponent->OnTookDamage.AddUObject(this, &UBsGameStatTracker::AddToDamageTaken));
		CharacterHandles.Add(HealthComponent->OnHealedDamage.AddUObject(this, &UBsGameStatTracker::AddToHealthRestored));
	}
}

void UBsGameStatTracker::StopTrackingPlayer()
{
	for (auto& Handle : CharacterHandles)
	{
		Handle.Reset();
	}
	CharacterHandles.Empty();
	
	for (auto& Handle : DynamicHandles)
	{
		Handle.RemoveAll(this);
	}
	DynamicHandles.Empty();

	for (auto& Handle : WeaponHandles)
	{
		Handle.Reset();
	}
	WeaponHandles.Empty();

	PlayerCharacter.Reset();
}