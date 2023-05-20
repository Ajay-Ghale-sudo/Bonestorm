// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile/BsProjectileBase.h"
#include "BsGrappleProjectile.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBsGrappleProjectileAttachedEvent, FVector, Location);
/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsGrappleProjectile : public ABsProjectileBase
{
	GENERATED_BODY()
	
public:
	
	FBsGrappleProjectileAttachedEvent OnGrappleComponentHit;
	
protected:

	virtual void OnProjectileOverlapInternal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
