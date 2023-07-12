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
	/**
	 * @brief Binds stat tracking to the player character's events.
	 */
	void StartTrackingPlayer();

	/**
	 * @brief Unbinds stat tracking from the player character's events.
	 */
	void StopTrackingPlayer();

	/**
	 * @brief Increments the given stat by 1.
	 * @param Stat The stat to increment.
	 */
	UFUNCTION()
	void IncrementStat(EBsPlayerStats Stat);

	/**
	 * @brief Adds the given amount to the given stat.
	 * @param Stat The stat to add to.
	 * @param Amount The amount to add to the stat.
	 */
	UFUNCTION()
	void AddToStat(EBsPlayerStats Stat, float Amount);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @brief Binds stat tracking to the given weapon's events.
	 * @param Weapon Weapon to bind to.
	 */
	UFUNCTION()
	void BindToWeapon(ABsWeaponBase* Weapon);

	/**
	 * @brief Binds stat tracking to the given projectile's events.
	 * @param Projectile Projectile to bind to.
	 */
	UFUNCTION()
	void BindToProjectile(ABsProjectileBase* Projectile);

	/**
	 * @brief Increments the deaths stat by 1.
	 */
	UFUNCTION()
	FORCEINLINE void IncrementDeaths() { IncrementStat(Deaths); }

	/**
	 * @brief Adds to DamageTaken stat.
	 * @param Amount Amount to add to DamageTaken.
	 */
	UFUNCTION()
	FORCEINLINE void AddToDamageTaken(const float Amount) { AddToStat(DamageTaken, Amount); }

	/**
	 * @brief Adds to HealthRestored stat.
	 * @param Amount Amount to add to HealthRestored.
	 */
	UFUNCTION()
	FORCEINLINE void AddToHealthRestored(const float Amount) { AddToStat(HealthRestored, Amount); }

	/**
	 * @brief Event for when a projectile damages an actor. Used to track damage dealt.
	 * @param Actor Actor that was damaged.
	 */
	UFUNCTION()
	void OnProjectileDamagedActor(AActor* Actor);

protected:
	/**
	 * @brief The player character to track stats for.
	 */
	UPROPERTY()
	TSoftObjectPtr<ABsCharacter> PlayerCharacter;

	/**
	 * @brief The player stats.
	 */
	FBsPlayerStats PlayerStats;

	/**
	 * @brief The name of the increment stat function.
	 */
	const FName IncrementStatFunctionName = FName("IncrementStat");

	/**
	 * @brief The name of the add to stat function.
	 */
	const FName AddToStatFunctionName = FName("AddToStat");

	/**
	 * @brief Currently bound character handles.
	 */
	TArray<FDelegateHandle> CharacterHandles;

	/**
	 * @brief Currently bound weapon handles.
	 */
	TArray<FDelegateHandle> WeaponHandles;

	/**
	 * @brief Currently bound dynamic handles.
	 */
	TArray<TBaseDynamicMulticastDelegate<FWeakObjectPtr, void>> DynamicHandles;

public:
	/**
	 * @brief Gets the player stats.
	 * @return The player stats.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Stats")
	FORCEINLINE FBsPlayerStats GetPlayerStats() const { return PlayerStats; }
};