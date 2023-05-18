// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/ReceiveDamage.h"
#include "BsEnemyBase.generated.h"

class ABsWeaponBase;
class UBsHealthComponent;
UCLASS()
class BOOMERSHOOTER_API ABsEnemyBase : public ACharacter, public IReceiveDamage
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABsEnemyBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// IReceive Damage Interface \\

	virtual void ReceiveProjectileDamage(const FHitResult& HitResult, ABsProjectileBase* Projectile,
	                                     const float Damage) override;
	virtual void ReceiveMeleeDamage(const FHitResult& HitResult, ABsWeaponBase* AttackingWeapon, const float Damage) override;
	virtual void ReceiveHazardDamage(ABsHazardBase* Hazard, const float Damage) override;
	//	                   		 \\

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Die();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UBsHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bIsAlive = true;
	
public:
	FORCEINLINE bool GetIsAlive() const { return bIsAlive; }
};
