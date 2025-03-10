﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/BsHealthComponent.h"
#include "Data/BsDamageType.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile/BsExplosiveProjectile.h"

// Sets default values for this component's properties
UBsHealthComponent::UBsHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBsHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (AActor *Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UBsHealthComponent::ProcessDamage);
	}
}

void UBsHealthComponent::ApplyDamage(float Damage, const UDamageType* DamageType)
{
	OnTookDamageEvent.Broadcast();
	const float ActualDamage = Damage * DamageModifier;
	OnTookDamage.Broadcast(ActualDamage);
	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.f, MaxHealth);

	if (CurrentHealth <= 0.f)
	{
		if (DamageType && DamageType->IsA(UBsDecapitateDamageType::StaticClass()))
		{
			OnDecapitated.Broadcast();
		}

		OnDeath.Broadcast();
		BleedTimerHandle.Invalidate();
		CurrentBleedDamageType = nullptr;
	}
	else if (IsLowHealth())
	{
		OnLowHealth.Broadcast();
	}
	OnHealthChanged.Broadcast();
	
	if (LastDamagedBy.IsValid())
	{
		OnDamagedBy.Broadcast(LastDamagedBy.Get());
	}
}

void UBsHealthComponent::ProcessDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                       AController* InstigatedBy, AActor* DamageCauser)
{
	// TODO: Damage Type should determine if self damage is allowed
	if (!DamagedActor || DamageCauser == DamagedActor || (DamageCauser && DamagedActor == DamageCauser->GetOwner()) || !DamageType)
	{
		return;
	}
	
	if (DamageTypesToIgnore.Contains(DamageType->GetClass()))
	{
		return;
	}

	LastDamagedBy = DamageCauser;
	
	if (const UBsBleedDamageType* BleedDamageType = Cast<UBsBleedDamageType>(DamageType))
	{
		CurrentBleedDamageType = BleedDamageType;
		CurrentBleedDuration = 0.f;
		ProcessBleedDamage();
	}
	else if (const UBsExplosionDamageType* ExplosionDamageType = Cast<UBsExplosionDamageType>(DamageType))
	{
		ABsExplosiveProjectile* Projectile = Cast<ABsExplosiveProjectile>(DamageCauser);
		const AActor* CurrentOwner = GetOwner();
		if (CurrentOwner && Projectile)
		{
			FVector LaunchVector = CurrentOwner->GetActorLocation() - Projectile->GetActorLocation();
			LaunchVector.Normalize();
			LaunchVector *= Projectile->GetExplosionImpulse();
			
			OnExplosionHit.Broadcast(LaunchVector);
		}
	}
	
	ApplyDamage(Damage, DamageType);
}

void UBsHealthComponent::ProcessBleedDamage()
{
	BleedTimerHandle.Invalidate();
	if (CurrentBleedDamageType)
	{
		CurrentBleedDuration += CurrentBleedDamageType->GetBleedInterval();
		if (CurrentBleedDuration < CurrentBleedDamageType->GetBleedDuration())
		{
			GetWorld()->GetTimerManager().SetTimer(
				BleedTimerHandle,
				this,
				&UBsHealthComponent::ProcessBleedDamage,
				CurrentBleedDamageType->GetBleedInterval(),
				false
				);
			ApplyDamage(CurrentBleedDamageType->GetBleedDamage());
		}
		else
		{
			CurrentBleedDuration = 0.f;
			CurrentBleedDamageType = nullptr;
		}
	}
}

void UBsHealthComponent::Heal(float Healing)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Healing, 0.f, MaxHealth);
	OnHealedDamage.Broadcast(Healing); // TODO: Should this broadcast the actual amount healed?
	OnHealthChanged.Broadcast();
}

// Called every frame
void UBsHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}