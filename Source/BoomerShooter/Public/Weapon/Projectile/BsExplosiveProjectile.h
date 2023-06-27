// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BsProjectileBase.h"
#include "GameFramework/Actor.h"
#include "BsExplosiveProjectile.generated.h"

class URadialForceComponent;
class UNiagaraSystem;
class UNiagaraComponent;

/**
 * @class ABsExplosiveProjectile
 * @brief A type of projectile that causes an explosion upon impact.
 *
 * This class defines an explosive projectile. When this type of projectile impacts with another actor,
 * it causes an explosion, applying damage and a physics impulse to nearby objects.
 */
UCLASS()
class BOOMERSHOOTER_API ABsExplosiveProjectile : public ABsProjectileBase
{
	GENERATED_BODY()

public:

	/**
	 * @brief Constructor that sets default values for this actor's properties.
	 */
	ABsExplosiveProjectile();

protected:

	/**
	 * @brief Overridden from AActor. Called when the game starts or when this actor is spawned into the game world.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Initialize visual and audio effects related to this projectile.
	 */
	void InitFX();

	/**
	 * @brief Show visual and audio effects related to the projectile's movement.
	 */
	void ShowProjectileFX() const;

	/**
	 * @brief Show visual and audio effects related to the projectile's impact and explosion.
	 */
	void ShowImpactFX() const;

	/**
	 * @brief Create an explosion at the current location of this actor.
	 */
	void CreateExplosion();

	/**
	 * @brief Overridden from ABsProjectileBase. Called when this projectile impacts with another actor.
	 */
	virtual void Impact() override;

protected:

	/**
	 * @brief Component that applies a radial physics impulse.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile|Explosion")
	URadialForceComponent* ExplosionForceComponent;

	/**
	 * @brief The radius of the explosion caused by this projectile, in Unreal units.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Explosion")
	float ExplosionRadius = 500.0f;

	/**
	 * @brief The strength of the physics impulse applied by this projectile's explosion.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile|Explosion")
	float ExplosionImpulse = 1000.0f;

	/**
	 * @brief The visual effect for the projectile's movement.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile|FX")
	UNiagaraSystem* ProjectileFX;

	/**
	 * @brief The component managing the projectile's movement visual effect.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|FX")
	UNiagaraComponent* ProjectileFXComponent;

	/**
	 * @brief The visual effect for the projectile's impact.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile|FX")
	UNiagaraSystem* ImpactFX;

	/**
	 * @brief The component managing the projectile's impact visual effect.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|FX")
	UNiagaraComponent* ImpactFXComponent;

public:

	/**
	 * @brief Get the explosion radius.
	 * @return The radius of the explosion caused by this projectile, in Unreal units.
	 */
	FORCEINLINE float GetExplosionRadius() const { return ExplosionRadius; }

	/**
	 * @brief Get the strength of the explosion impulse.
	 * @return The strength of the physics impulse applied by this projectile's explosion.
	 */
	FORCEINLINE float GetExplosionImpulse() const { return ExplosionImpulse; }
};