// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BsWeaponBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FBsWeaponBaseEvent);

UCLASS()
class BOOMERSHOOTER_API ABsWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsWeaponBase();

	UFUNCTION(BlueprintCallable)
	virtual void Fire();
	UFUNCTION(BlueprintCallable)
	virtual void SecondaryFire();
	
	UFUNCTION(BlueprintCallable)
	virtual void NextWeaponMode();

	/**
	 * @brief Drops the Weapon in the world.
	 */
	void Drop();

	/**
	 * @brief Throws the Weapon.
	 */
	virtual void Throw();

	/**
	 * @brief Equip the Weapon.
	 */
	virtual void Equip();

public:
	FBsWeaponBaseEvent OnWeaponCaught;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void EnableAttack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackRate = 0.1f;

	FTimerHandle AttackTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bCanAttack = true;

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
};
