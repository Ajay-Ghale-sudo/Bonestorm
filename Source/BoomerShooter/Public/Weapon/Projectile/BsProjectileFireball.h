// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BsProjectileBase.h"
#include "BsProjectileFireball.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class BOOMERSHOOTER_API ABsProjectileFireball : public ABsProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsProjectileFireball();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnProjectileOverlapInternal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;
	
	void Impact();

	void InitFX();
	void ShowProjectileFX();
	void ShowImpactFX();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile|FX")
	UNiagaraSystem* ProjectileFX;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Projectile|FX")
	UNiagaraComponent* ProjectileFXComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile|FX")
	FName ProjectileFXAttachPoint = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile|FX")
	UNiagaraSystem* ProjectileImpactFX;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Projectile|FX")
	UNiagaraComponent* ProjectileImpactFXComponent;

public:
};
