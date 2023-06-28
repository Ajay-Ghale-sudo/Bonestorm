// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interfaces/Decapitator.h"
#include "Weapon/BsWeaponBase.h"
#include "Weapon/Projectile/BsGrappleProjectile.h"
#include "BsScythe.generated.h"

class UBsGrappleHookComponent;
class UBoxComponent;
class ABsProjectileBase;

UENUM(BlueprintType)
enum class EScytheWeaponMode : uint8
{
	ESWM_Melee UMETA(DisplayName = "Melee"),
	ESWM_Range UMETA(DisplayName = "Range"),
	ESWM_Thrown UMETA(DisplayName = "Thrown"),

	ESWM_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FScytheRangedConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe|Ranged Config")
	float FireRate = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe|Ranged Config")
	TSubclassOf<ABsProjectileBase> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Scythe|Ranged Config")
	bool bCanFire = true;

	FTimerHandle FireRateTimerHandle;
};

USTRUCT(BlueprintType)
struct FBsScytheBlockConfig
{
	GENERATED_BODY()

	FTimerHandle ParryTimerHandle;
	FTimerHandle ParryCooldownHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block|Parry")
	float ParryDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block|Parry")
	float ParryCooldown = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block|Parry")
	float BlockingDamageReduction = 0.3f;

	// Charge reduction later multiplied by the damage of the hit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block|Parry")
	float BlockingChargeReduction = 1.25f;

	// Adds head charge based on the damage of the hit parried
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block|Parry")
	float ParryingChargeMultiplier = 1.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Block|Parry")
	bool bCanParry = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Block|Parry")
	bool bBlocking = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Block|Parry")
	bool bParrying = false;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FBsScytheBlockEvent, float Damage)
/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsScythe : public ABsWeaponBase, public IDecapitator
{
	GENERATED_BODY()

public:
	ABsScythe();
	
	virtual void Fire() override;
	virtual void SecondaryFire() override;
	
	void RangeAttack();
	void MeleeAttack();
	void SecondaryAttack();

	/**
	 * Blueprint Events
	 */

	UFUNCTION(BlueprintNativeEvent)
	void OnMeleeAttack();

	UFUNCTION(BlueprintNativeEvent)
	void OnRangedAttack();

	UFUNCTION(BlueprintNativeEvent)
	void OnSecondaryAttack();
	
	/**
	 * @brief Sets the current weapon mode
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeaponMode(EScytheWeaponMode NewMode);
	
	virtual void NextWeaponMode() override;
	
	UFUNCTION(BlueprintCallable)
	void SetAttacking(bool bNewAttacking);

	/**
	 * @brief Functions determining logic on grapple event
	 */
	UFUNCTION()
	void StartGrappling();

	UFUNCTION()
	void StopGrappling();

	virtual void Throw() override;
	virtual void Equip() override;

	virtual void DecapitatedActor(ABsSeveredHeadBase* DecapitatedHead) override;

public:
	FBsScytheBlockEvent OnScytheParryEvent;
	FBsScytheBlockEvent OnScytheBlockEvent;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void ThrowTick(float DeltaTime);
	virtual bool CanAttack() const override;
	virtual float BlockIncomingDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void StartBlock() override;
	virtual void StopBlock() override;
	void StartParry();
	UFUNCTION()
	void StopParry();
	UFUNCTION()
	void EnableParry();
	void EnableRangedFire();

	void MeleeAttackFinished();
	
	UFUNCTION()
	void OnGrappleHookDetached();
	
	virtual FTransform GetProjectileSpawnTransform() const;
	
	UFUNCTION()
	void OnScytheOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnMeleeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scythe")
	UBsGrappleHookComponent* GrappleHookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	EScytheWeaponMode WeaponMode = EScytheWeaponMode::ESWM_Melee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	EScytheWeaponMode RangedWeaponMode = EScytheWeaponMode::ESWM_Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	float MeleeDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scythe")
	UBoxComponent* MeleeCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scythe")
	UBoxComponent* ScytheCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	float AttackDuration = 0.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scythe")
	bool bGrappling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe|Montage")
	UAnimMontage* ThrowMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe|Montage")
	UAnimMontage* MeleeAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe|Montage")
	UAnimMontage* RangedAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe|Montage")
	UAnimMontage* BlockMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe|Montage")
    UAnimMontage* ParryMontage;

	bool bThrown = false;
	bool bReturningToOwner = false;
	bool bAttachedToGrapplePoint = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	float ThrowSpeed = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	float ThrowDistance = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scythe")
	TSubclassOf<UDamageType> ThrownDamageType = UDamageType::StaticClass();

	/**
	 * @brief Distance before the scythe is returned to the owner.
	 */
	float ReturnDistanceThreshold = 100.f;

	FVector ThrowDirection = FVector::ZeroVector;
	FVector ThrowStartLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	FScytheRangedConfig RangedConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	FBsScytheBlockConfig BlockConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scythe")
	TArray<AActor*> MeleeHitActors;

public:
	FORCEINLINE bool IsGrappling() const { return bGrappling; }
	FORCEINLINE bool IsThrown() const { return bThrown; }
	FORCEINLINE bool IsReturningToOwner() const { return bReturningToOwner; }
	FORCEINLINE bool IsAttachedToGrapplePoint() const { return bAttachedToGrapplePoint; }
	FORCEINLINE bool IsMeleeMode() const { return WeaponMode == EScytheWeaponMode::ESWM_Melee; }
	FORCEINLINE bool IsRangedMode() const { return WeaponMode == EScytheWeaponMode::ESWM_Range; }
	FORCEINLINE EScytheWeaponMode GetWeaponMode() const { return WeaponMode; }
};