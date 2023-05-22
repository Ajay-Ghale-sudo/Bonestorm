// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Component/BsGrapplePointComponent.h"
#include "Weapon/Projectile/BsProjectileBase.h"
#include "BsGrappleProjectile.generated.h"


class UBsGrapplePointComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsGrappledProjectileEvent);
/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsGrappleProjectile : public ABsProjectileBase
{
	GENERATED_BODY()

	
public:
	ABsGrappleProjectile();
	
	void Detach();
	
public:
	
	FBsGrappledProjectileEvent OnGrappleComponentAttached;
	FBsGrappledProjectileEvent OnGrappleComponentDetached;
	
	
protected:
	virtual void BeginPlay() override;
	
	virtual void OnProjectileOverlapInternal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void AttachToGrappleComponent(const UBsGrapplePointComponent* GrapplePoint);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grapple")
	UBsGrapplePointComponent* AttachedGrapplePoint = nullptr;


public:
	FORCEINLINE bool IsAttached() const { return AttachedGrapplePoint != nullptr; }
	FORCEINLINE UBsGrapplePointComponent* GetAttachedGrapplePointComponent() const { return AttachedGrapplePoint; }
	FORCEINLINE AActor* GetGrapplePointActor() const { return AttachedGrapplePoint ? AttachedGrapplePoint->GetOwner() : nullptr; }
	
};
