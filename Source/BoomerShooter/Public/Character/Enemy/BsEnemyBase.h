// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Interfaces/ReceiveDamage.h"
#include "BsEnemyBase.generated.h"

class ABsSeveredHeadBase;
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

	UFUNCTION(BlueprintCallable)
	virtual bool CanAttack() const;

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

	UFUNCTION(BlueprintCallable)
	virtual void TriggerRagdoll();

	virtual void SeverHead();

	void EnableAttack();
	virtual void OnAttack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UBsHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<ABsSeveredHeadBase> SeveredHeadClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bIsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bCanAttack = true;

	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float AttackCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	FName RootBoneName = FName("Pelvis");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	FName HeadBoneName = FName("Head");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float RagdollPhysicsBlendWeight = 0.75f;
	
public:
	FORCEINLINE bool GetIsAlive() const { return bIsAlive; }
};