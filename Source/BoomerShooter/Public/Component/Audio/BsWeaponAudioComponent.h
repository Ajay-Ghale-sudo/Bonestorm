// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BsAudioComponentBase.h"
#include "BsWeaponAudioComponent.generated.h"

class ABsWeaponBase;
class USoundBase;

USTRUCT(BlueprintType)
struct FWeaponAudioDate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* WeaponCaughtSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* WeaponParrySound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* WeaponBlockSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* WeaponThrowSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* WeaponEquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* WeaponModeChangedSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* WeaponFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* WeaponHealSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* WeaponHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* WeaponMeleeAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* WeaponHeadConsumedSound;
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsWeaponAudioComponent : public UBsAudioComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBsWeaponAudioComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

	// Audio Event Handlers \\

	UFUNCTION()
	void OnWeaponCaught();
	
	UFUNCTION()
	void OnWeaponParry();

	UFUNCTION()
	void OnWeaponBlock();

	UFUNCTION()
	void OnWeaponThrow();

	UFUNCTION()
	void OnWeaponEquip();

	UFUNCTION()
	void OnWeaponModeChanged();

	UFUNCTION()
	void OnWeaponFire();
	
	UFUNCTION()
	void OnWeaponHit();

	UFUNCTION()
	void OnWeaponHeal();

	UFUNCTION()
	void OnMeleeAttack();

	UFUNCTION()
	void OnWeaponHeadConsumed();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component", meta = (AllowPrivateAccess = "true"))
	ABsWeaponBase* WeaponOwner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Component", meta = (AllowPrivateAccess = "true"))
	FWeaponAudioDate WeaponAudioData;
};