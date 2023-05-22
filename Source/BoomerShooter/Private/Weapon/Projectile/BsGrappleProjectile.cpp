// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/BsGrappleProjectile.h"
#include "Component/BsGrapplePointComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABsGrappleProjectile::ABsGrappleProjectile()
{

}

void ABsGrappleProjectile::BeginPlay()
{
	Super::BeginPlay();	
}

void ABsGrappleProjectile::OnProjectileOverlapInternal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if(const UBsGrapplePointComponent* GrapplePoint = OtherActor->FindComponentByClass<UBsGrapplePointComponent>())
		{
			AttachToGrappleComponent(GrapplePoint);
			return;
		}
	}
	
	Detach();
}

void ABsGrappleProjectile::AttachToGrappleComponent(const UBsGrapplePointComponent* GrapplePoint)
{
	SetActorEnableCollision(false);
	DisableComponentsSimulatePhysics();
	if (ProjectileCollision)
	{
		ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProjectileCollision->SetSimulatePhysics(false);
		ProjectileCollision->SetEnableGravity(false);
	}

	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->bSimulationEnabled = false;
		
	}
	OnGrappleComponentAttached.Broadcast();
}



void ABsGrappleProjectile::Detach()
{
	OnGrappleComponentDetached.Broadcast();
	AttachedGrapplePoint = nullptr;
	Destroy();
}

