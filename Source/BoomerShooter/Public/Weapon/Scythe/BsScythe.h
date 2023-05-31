// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsScythe : public ABsWeaponBase
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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void ThrowTick(float DeltaTime);
	virtual bool CanAttack() const override;
	void EnableRangedFire();
	
	virtual FTransform GetProjectileSpawnTransform() const;
	
	UFUNCTION()
	void OnScytheOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Scythe")
	bool bIsAttacking = false;

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

	bool bThrown = false;
	bool bReturningToOwner = false;
	bool bAttachedToGrapplePoint = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	float ThrowSpeed = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	float ThrowDistance = 3000.f;

	/**
	 * @brief Distance before the scythe is returned to the owner.
	 */
	float ReturnDistanceThreshold = 100.f;

	FVector ThrowDirection = FVector::ZeroVector;
	FVector ThrowStartLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	FScytheRangedConfig RangedConfig;

public:
	FORCEINLINE bool IsAttacking() const { return bIsAttacking; }
	FORCEINLINE bool IsGrappling() const { return bGrappling; }
	FORCEINLINE bool IsThrown() const { return bThrown; }
	FORCEINLINE bool IsReturningToOwner() const { return bReturningToOwner; }
	FORCEINLINE bool IsAttachedToGrapplePoint() const { return bAttachedToGrapplePoint; }
	FORCEINLINE bool IsMeleeMode() const { return WeaponMode == EScytheWeaponMode::ESWM_Melee; }
	FORCEINLINE bool IsRangedMode() const { return WeaponMode == EScytheWeaponMode::ESWM_Range; }
	FORCEINLINE EScytheWeaponMode GetWeaponMode() const { return WeaponMode; }
};
