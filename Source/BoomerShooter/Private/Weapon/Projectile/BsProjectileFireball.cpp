﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/BsProjectileFireball.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Component/BsHealthComponent.h"
#include "Engine/DamageEvents.h"
#include "Interfaces/ReceiveDamage.h"


// Sets default values
ABsProjectileFireball::ABsProjectileFireball()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABsProjectileFireball::BeginPlay()
{
	InitFX();
	ShowProjectileFX();
	
	Super::BeginPlay();
}

void ABsProjectileFireball::OnProjectileOverlapInternal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnProjectileOverlapInternal(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (ProjectileFXComponent)
	{
		ProjectileFXComponent->DestroyInstance();
	}
}


void ABsProjectileFireball::Impact()
{
	if (ProjectileImpactFXComponent)
	{
		ProjectileImpactFXComponent->Activate(true);
	}

	if (ProjectileFXComponent)
	{
		ProjectileFXComponent->SetVisibility(false);
		ProjectileFXComponent->DeactivateImmediate();
		ProjectileFXComponent->DestroyComponent();
	}
	
	Super::Impact();
}

void ABsProjectileFireball::InitFX()
{
	if (ProjectileFX)
	{
		ProjectileFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ProjectileFX,
			ProjectileMesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true,
			false,
			ENCPoolMethod::AutoRelease,
			true
		);
	}

	if (ProjectileImpactFX)
	{
		ProjectileImpactFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ProjectileImpactFX,
			ProjectileMesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true,
			false,
			ENCPoolMethod::AutoRelease,
			true
		);
	}
}

void ABsProjectileFireball::ShowProjectileFX()
{
	if (ProjectileFXComponent)
	{
		ProjectileFXComponent->Activate(true);
	}
}

void ABsProjectileFireball::ShowImpactFX()
{
	if (ProjectileImpactFXComponent)
	{
		ProjectileImpactFXComponent->Activate(true);
	}
}
