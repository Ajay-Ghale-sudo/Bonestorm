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
	void SetGrappling(bool bIsGrappling);

	UFUNCTION()
	void StartGrappling();

	UFUNCTION()
	void StopGrappling();



protected:
	virtual void BeginPlay() override;
	
	virtual FTransform GetProjectileSpawnTransform() const;
	
	UFUNCTION()
	void OnScytheOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	TSubclassOf<ABsProjectileBase> ProjectileClass;

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
};
