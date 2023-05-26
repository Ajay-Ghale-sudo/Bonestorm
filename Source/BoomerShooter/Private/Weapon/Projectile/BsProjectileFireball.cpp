// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/BsProjectileFireball.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"



// Sets default values
ABsProjectileFireball::ABsProjectileFireball()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABsProjectileFireball::BeginPlay()
{
	Super::BeginPlay();

	InitFX();
	ShowProjectileFX();
}



void ABsProjectileFireball::Impact()
{
	if (ProjectileImpactFXComponent)
	{
		ProjectileImpactFXComponent->Activate(true);
	}
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
