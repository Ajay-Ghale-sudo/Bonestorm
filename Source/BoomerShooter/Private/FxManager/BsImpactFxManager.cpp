// Fill out your copyright notice in the Description page of Project Settings.


#include "FxManager/BsImpactFxManager.h"

#include "NiagaraFunctionLibrary.h"
#include "Weapon/Projectile/BsProjectileBase.h"

void UBsImpactFxManager::Init()
{
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UBsImpactFxManager::WorldChanged);
	WorldChanged(GetWorld(),  UWorld::InitializationValues());
}

void UBsImpactFxManager::ResolveImpact(const FHitResult& HitResult)
{
	// Get the physical material from the hit and check it with the impact fx map
	const UPhysicalMaterial* HitPhysicalMaterial = HitResult.PhysMaterial.Get();
	if (ImpactFxMap.Contains(HitPhysicalMaterial))
	{
		// Spawn the impact fx
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactFxMap[HitPhysicalMaterial],
			HitResult.ImpactPoint,
			HitResult.ImpactNormal.Rotation(),
			FVector(0.3f),
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true			
		);
	}
}

void UBsImpactFxManager::WorldChanged(UWorld* World, const UWorld::InitializationValues IVS)
{
	if (World)
	{
		OnActorSpawnedHandle = World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &UBsImpactFxManager::OnActorSpawned));
	}
}

void UBsImpactFxManager::OnActorSpawned(AActor* SpawnedActor)
{
	if (ABsProjectileBase* Projectile = Cast<ABsProjectileBase>(SpawnedActor))
	{
		// Only process impact if damage was caused. This handles the case where projectiles are parried.
		Projectile->OnDealtDamage.AddUObject(this, &UBsImpactFxManager::ResolveImpact);
	}
}