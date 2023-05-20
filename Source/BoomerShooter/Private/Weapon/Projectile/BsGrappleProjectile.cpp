// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/BsGrappleProjectile.h"
#include "Component/BsGrappleComponent.h"

void ABsGrappleProjectile::OnProjectileOverlapInternal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if(const UBsGrappleComponent* GrappleComponent = OtherActor->FindComponentByClass<UBsGrappleComponent>())
		{
			const FVector GrappleHitLocation = GrappleComponent->GetComponentLocation();
			OnGrappleComponentHit.Broadcast(GrappleHitLocation);
		}
	}
}

