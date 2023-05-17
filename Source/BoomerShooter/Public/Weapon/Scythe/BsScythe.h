// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/BsWeaponBase.h"
#include "BsScythe.generated.h"


UENUM(BlueprintType)
enum class EScytheWeaponMode : uint8
{
	ESWM_Melee UMETA(DisplayName = "Melee"),
	ESWM_Range UMETA(DisplayName = "Range"),

	ESWM_MAX UMETA(DisplayName = "DefaultMAX")
};

class ABsProjectileBase;

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsScythe : public ABsWeaponBase
{
	GENERATED_BODY()


public:

	virtual void Fire() override;
	
	void RangeAttack();
	void MeleeAttack();

	UFUNCTION(BlueprintNativeEvent)
	void OnMeleeAttack();

	UFUNCTION(BlueprintNativeEvent)
	void OnRangedAttack();

	UFUNCTION(BlueprintCallable)
	void SetWeaponMode(EScytheWeaponMode NewMode);
	

	virtual void NextWeaponMode() override;



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	TSubclassOf<ABsProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	EScytheWeaponMode WeaponMode = EScytheWeaponMode::ESWM_Melee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scythe")
	EScytheWeaponMode RangedWeaponMode = EScytheWeaponMode::ESWM_Range;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EScytheWeaponMode GetWeaponMode() const;
	
};
