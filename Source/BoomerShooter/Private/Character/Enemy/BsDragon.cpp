// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsDragon.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile/BsProjectileBase.h"

void ABsDragon::RangeAttack(const ACharacter* Target)
{
	UWorld* World = GetWorld();
	if (Target && ProjectileClass && World && GetMesh())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const FTransform TargetTransform = Target->GetActorTransform();
		const FTransform SpawnTransform = GetMesh()->GetSocketTransform(FName("RangedAttackSocket"));
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnTransform.GetLocation(), TargetTransform.GetLocation());

		FHitResult LOSHitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(Target);

		// Is anything in the way of us and the target?
		bool bHasLOS = !World->LineTraceSingleByChannel(
			LOSHitResult,
			SpawnTransform.GetLocation(),
			 TargetTransform.GetLocation(),
			 ECollisionChannel::ECC_Visibility,
			 QueryParams
		);

		if (!bHasLOS)
		{
			// TODO: This should return a bool or notify the failure somehow.
			return;
		}
		
		if (ABsProjectileBase* Projectile = World->SpawnActor<ABsProjectileBase>(ProjectileClass, SpawnTransform.GetLocation(), SpawnRotation, SpawnParameters))
		{
			if (UProjectileMovementComponent* ProjectileMovement = Projectile->GetProjectileMovement())
			{
				const float ProjectileSpeed = Projectile->GetProjectileDamageProperties().ProjectileSpeed;
				if (ProjectileSpeed <= 0) return;
				
				// (Dist to target / bullet speed) * (target speed * target forward vec + target location)
				const float TravelTime = FVector::Dist(TargetTransform.GetLocation(), SpawnTransform.GetLocation()) / ProjectileSpeed;
				FVector TargetLocation = TargetTransform.GetLocation() + (Target->GetVelocity() * TravelTime);
				
				// How Long it would take us to hit the target where it currently is, provides us a time window to calculate the target's future position
				const float TravelDistTime = FVector::Dist(TargetLocation, SpawnTransform.GetLocation()) / ProjectileSpeed;
				TargetLocation = TargetTransform.GetLocation() + (Target->GetVelocity() * TravelDistTime);
				
				FVector OutLaunchVelocity;
				bool bHasAimSolution = UGameplayStatics::SuggestProjectileVelocity(
					World,
					OutLaunchVelocity,
					SpawnTransform.GetLocation(),
					TargetLocation,
					ProjectileSpeed,
					false,
					10.f,
					ProjectileMovement->GetGravityZ(),
					ESuggestProjVelocityTraceOption::DoNotTrace,
					FCollisionResponseParams::DefaultResponseParam,
					TArray<AActor*>(),
					false
					);

				if (bHasAimSolution)
				{
					ProjectileMovement->Velocity = OutLaunchVelocity;
				}
				else
				{
					// Cannot hit target, just fire in a straight line
				}
			}
				
		}
	}
}
