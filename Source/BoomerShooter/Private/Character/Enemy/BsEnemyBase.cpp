// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsEnemyBase.h"

#include "Component/BsHealthComponent.h"
#include "Engine/DamageEvents.h"
#include "Weapon/BsWeaponBase.h"
#include "Weapon/Projectile/BsProjectileBase.h"


// Sets default values
ABsEnemyBase::ABsEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UBsHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABsEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ABsEnemyBase::Die);
	}
	
}

void ABsEnemyBase::Die()
{
	
}

// Called every frame
void ABsEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABsEnemyBase::ReceiveProjectileDamage(const FHitResult& HitResult, ABsProjectileBase* Projectile,
	const float Damage)
{
	if (!Projectile) return;

	// TODO: DamageEvent should come from the projectile
	TakeDamage(Damage, FDamageEvent(), Projectile->GetInstigatorController(), Cast<AActor>(Projectile));
}

void ABsEnemyBase::ReceiveMeleeDamage(const FHitResult& HitResult, ABsWeaponBase* AttackingWeapon, const float Damage)
{
	if (!AttackingWeapon) return;
	
	// TODO: DamageEvent should come from the AttackingWeapon
	TakeDamage(Damage, FDamageEvent(), AttackingWeapon->GetInstigatorController(), AttackingWeapon);
}


