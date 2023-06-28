// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BsProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;


DECLARE_MULTICAST_DELEGATE_OneParam(FBsProjectileHitEvent, const FHitResult& /*HitResult*/);

USTRUCT(BlueprintType)
struct FProjectileDamageProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileLifeTime = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<UDamageType> ProjectileDamageType = UDamageType::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	bool bSupportsCheckProjectilePath = false;

	FTimerHandle ImpactTimerHandle;
	FTimerHandle ResolveImpactTimerHandle;
};

UCLASS()
class BOOMERSHOOTER_API ABsProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsProjectileBase();

	void SetDamageType(TSubclassOf<UDamageType> DamageType);

public:
	FBsProjectileHitEvent OnDealtDamage;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool CheckProjectilePath();

	/**
	 * @brief Resolves the impact. Should be called after Impact.
	 */
	void ResolveImpact();

	UFUNCTION()
	virtual void Impact();

	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileHit(UPrimitiveComponent* OnComponentHit, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	                     const FHitResult& Hit);

	virtual void OnProjectileHitInternal(UPrimitiveComponent* OnComponentHit, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
						 const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                         bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnProjectileOverlapInternal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							 bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ProjectileParried(AActor* DamageCauser);

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void SetProjectileCollision(ECollisionEnabled::Type CollisionEnabled);

	void UpdateMoveActorIgnore();

	float ApplyDamageToActor(AActor* OtherActor);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	USphereComponent* ProjectileCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FProjectileDamageProperties ProjectileDamageProperties;

	bool bParried = false;

	FHitResult LastHitResult;

	/**
	 * @brief Amount of damage dealt by this projectile.
	 */
	float DamageDealt = 0.f;

public:
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	FORCEINLINE FProjectileDamageProperties GetProjectileDamageProperties() const { return ProjectileDamageProperties; }
	FORCEINLINE USphereComponent* GetProjectileCollision() const { return ProjectileCollision; }
	FORCEINLINE float GetProjectileDamage() const { return ProjectileDamageProperties.ProjectileDamage; }
};