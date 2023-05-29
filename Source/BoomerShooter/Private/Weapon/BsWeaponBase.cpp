// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BsWeaponBase.h"


// Sets default values
ABsWeaponBase::ABsWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void ABsWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABsWeaponBase::EnableAttack()
{
	bCanAttack = true;
}

void ABsWeaponBase::Fire()
{
	if (!bCanAttack)
	{
		return;
	}
	
	bCanAttack = false;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ABsWeaponBase::EnableAttack, AttackRate, false);
}

void ABsWeaponBase::SecondaryFire()
{
	if (!bCanAttack)
	{
		return;
	}

	bCanAttack = false;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ABsWeaponBase::EnableAttack, AttackRate, false);
}

void ABsWeaponBase::NextWeaponMode()
{
		
}

void ABsWeaponBase::Drop()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (WeaponMesh)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
		WeaponMesh->SetPhysicsBlendWeight(0.f);
		WeaponMesh->SetSimulatePhysics(true);
	}
}