// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BsImpactFxManager.generated.h"

class UNiagaraSystem;
class UPhysicalMaterial;

/**
 * 
 */
UCLASS(Blueprintable)
class BOOMERSHOOTER_API UBsImpactFxManager : public UObject
{
	GENERATED_BODY()

public:
	void Init();

	UFUNCTION()
	void ResolveImpact(const FHitResult& HitResult);
	void WorldChanged(UWorld* World, const UWorld::InitializationValues IVS);
	
protected:
	UFUNCTION()
	void OnActorSpawned(AActor* SpawnedActor);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact Fx")
	TMap<UPhysicalMaterial*, UNiagaraSystem*> ImpactFxMap;

	FDelegateHandle OnActorSpawnedHandle;
};