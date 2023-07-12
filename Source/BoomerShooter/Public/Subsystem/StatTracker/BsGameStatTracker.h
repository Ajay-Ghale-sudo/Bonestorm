// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BsGameStatTracker.generated.h"

class ABsWeaponBase;
class ABsCharacter;
class ABsProjectileBase;

UENUM()
enum EBsPlayerStats : uint8
{
	Unknown,
	
	Jumps,
	DashJumps,
	SlideJumps,
	Dashes,
	Slides,
	Grapples,
	Kills,
	Deaths,
	ShotsFired,
	RangeShotsHit,
	MeleeSwings,
	DamageDealt,
	DamageTaken,
	HealthRestored,
	HeadsAttached,
	HeadsConsumed,
	
	Max
};

USTRUCT(BlueprintType)
struct FBsPlayerStats
{
	GENERATED_BODY()

	TMap<EBsPlayerStats, float> Stats;
};

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsGameStatTracker : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void StartTrackingPlayer();
	void StopTrackingPlayer();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS);

	UFUNCTION()
	void IncrementStat(EBsPlayerStats Stat);

	UFUNCTION()
	void AddToStat(EBsPlayerStats Stat, float Amount);

	UFUNCTION()
	void BindToWeapon(ABsWeaponBase* Weapon);

	UFUNCTION()
	FORCEINLINE void IncrementDeaths() { IncrementStat(Deaths); }

	UFUNCTION()
	FORCEINLINE void AddToDamageTaken(const float Amount) { AddToStat(DamageTaken, Amount); }

	UFUNCTION()
	FORCEINLINE void AddToHealthRestored(const float Amount) { AddToStat(HealthRestored, Amount); }

	UFUNCTION()
	void BindToProjectile(ABsProjectileBase* Projectile);

	UFUNCTION()
	void OnProjectileDamagedActor(AActor* Actor);

	
protected:
	UPROPERTY()
	TSoftObjectPtr<ABsCharacter> PlayerCharacter;

	FBsPlayerStats PlayerStats;

	const FName IncrementStatFunctionName = FName("IncrementStat");
	const FName AddToStatFunctionName = FName("AddToStat");

	TArray<FDelegateHandle> WeaponHandles;

public:
	UFUNCTION(BlueprintCallable, Category = "Game Stats")
	FORCEINLINE FBsPlayerStats GetPlayerStats() const { return PlayerStats; }
};