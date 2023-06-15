﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BsWeaponBase.generated.h"

class ABsSeveredHeadBase;
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

	/**
	 * @brief Attaches a SeveredHead to this weapon.
	 * @param SeveredHead The SeveredHead to attach.
	 */
	void AttachSeveredHead(ABsSeveredHeadBase* SeveredHead);

	/**
	 * @brief Detaches the currently Attached SeveredHead.
	 */
	UFUNCTION()
	void DetachSeveredHead();

public:
	FBsWeaponBaseEvent OnWeaponCaught;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void EnableAttack();

	/**
	 * @brief Plays the Montage on the Weapon Mesh.
	 * @param MontageToPlay Montage to play.
	 */
	void PlayMontage(UAnimMontage* MontageToPlay) const;

	/**
	 * @brief Clears the Montage on the Weapon Mesh.
	 * @param MontageToClear Montage to clear. If NULL, all Montages will be cleared.
	 */
	void ClearMontage(const UAnimMontage* MontageToClear = nullptr) const;

	/**
	 * @brief Get if the Weapon can attack.
	 * @return Returns True if the Weapon can attack.
	 */
	virtual bool CanAttack() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackRate = 0.1f;

	FTimerHandle AttackTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector SeveredHeadScale = FVector(0.5f, 0.5f, 0.5f); // TODO: Should this be handled on a per head basis?
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	ABsSeveredHeadBase* AttachedSeveredHead;

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
};
