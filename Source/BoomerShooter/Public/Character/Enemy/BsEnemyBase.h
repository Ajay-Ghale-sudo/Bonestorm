// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Data/AttackResult.h"
#include "GameFramework/Character.h"
#include "Interfaces/ReceiveDamage.h"
#include "BsEnemyBase.generated.h"

class UBsEnemyAudioComponent;
class UBoxComponent;
class ABsSeveredHeadBase;
class ABsWeaponBase;
class UBsHealthComponent;
class UAnimMontage;
class UMaterialInstance;

DECLARE_MULTICAST_DELEGATE(FBsEnemyEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBsEnemyEventWithCaller, ABsEnemyBase*, Enemy);

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

	UFUNCTION(BlueprintCallable)
	virtual EAttackResult MeleeAttack();

	UFUNCTION(BlueprintCallable)
	virtual bool CanMeleeAttackTarget(AActor* Target);

public:
	FBsEnemyEvent OnHitStun;
	FBsEnemyEvent OnDeath;
	FBsEnemyEventWithCaller OnThisEnemyDeath;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Die();

	UFUNCTION(BlueprintCallable)
	virtual void TriggerRagdoll();

	UFUNCTION()
	virtual void SeverHead();

	void EnableAttack();
	virtual void Attack();
	virtual void OnAttack();
	virtual void BindMeleeHitBox();
	
	/**
	 * @brief Launch this enemy from an explosion.
	 * @param LaunchVector The direction and magnitude to launch this enemy.
	 */
	UFUNCTION()
	virtual void ExplosionLaunch(FVector LaunchVector);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetMeleeHitBoxEnabled(bool bEnabled);

	UFUNCTION()
	virtual void StartHitStun();

	UFUNCTION()
	virtual void ApplyHitStun(float Duration);

	UFUNCTION()
	virtual void EndHitStun();

	UFUNCTION()
	virtual void IndicateLowHealth();

	UFUNCTION()
	virtual void ClearOverlayMaterial();
	
	void PlayMontage(UAnimMontage* MontageToPlay) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UBsHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UBsEnemyAudioComponent* EnemyAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UBoxComponent* MeleeHitBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<ABsSeveredHeadBase> SeveredHeadClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bIsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bCanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bHitStunned = false;

	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float AttackCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float MeleeAttackRange = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float MeleeAttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	FName RootBoneName = FName("Pelvis");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	FName HeadBoneName = FName("Head");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float RagdollPhysicsBlendWeight = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UAnimMontage* HitStunMontage;

	FTimerHandle HitStunTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float HitStunDuration = 0.3f;

	float PreHitStunMaxWalkSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float LowHealthHitStunDuration = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UMaterialInstance* LowHealthMaterial;
	
public:
	FORCEINLINE bool GetIsAlive() const { return bIsAlive; }
	FORCEINLINE UBsHealthComponent* GetHealthComponent() const { return HealthComponent; }
};