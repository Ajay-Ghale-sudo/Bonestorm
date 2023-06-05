// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsDragon.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile/BsProjectileBase.h"

bool ABsDragon::TargetInLOS(const AActor* Target) const
{
	UWorld* World = GetWorld();
	if (!Target || !World) return false;

	TArray<AActor*> ActorsToIgnore;
	Target->GetAttachedActors(ActorsToIgnore);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Target);
	Params.AddIgnoredActors(ActorsToIgnore);	
	
	FHitResult HitResult;
	World->LineTraceSingleByChannel(
		HitResult,
		GetActorLocation(),
		Target->GetActorLocation(),
		ECollisionChannel::ECC_Visibility,
		Params
	);

	if (HitResult.bBlockingHit)
	{
		// TODO: If we want to be able to shoot through objects we need to check if the hit actor here and then recast
		return false;
	}

	// We have a clear line of sight to the target
	return true;
	
}

bool ABsDragon::TargetInRange(const AActor* Target) const
{
	if (!Target) return false;

	return LocationInRange(Target->GetTargetLocation());
}

bool ABsDragon::LocationInRange(const FVector& Location) const
{
	const float Distance = FVector::Dist(GetActorLocation(), Location);
	return  Distance <= MaxAttackRange && Distance >= MinAttackRange;
}

bool ABsDragon::CanAttackTarget(const AActor* Target) const
{
	return bCanAttack && TargetInLOS(Target) && TargetInRange(Target);
}

EAttackResult ABsDragon::RangeAttack(const ACharacter* Target)
{
	if (!TargetInRange(Target))
	{
		return EAttackResult::EAR_TooFar;
	}

	if (!TargetInLOS(Target))
	{
		return EAttackResult::EAR_NoLOS;
	}
	
	UWorld* World = GetWorld();
	if (CanAttack() && Target && ProjectileClass && World && GetMesh())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const FTransform TargetTransform = Target->GetActorTransform();
		const FTransform SpawnTransform = GetMesh()->GetSocketTransform(FName("RangedAttackSocket"));
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnTransform.GetLocation(), TargetTransform.GetLocation());

		
		if (ABsProjectileBase* Projectile = World->SpawnActor<ABsProjectileBase>(ProjectileClass, SpawnTransform.GetLocation(), SpawnRotation, SpawnParameters))
		{
			if (UProjectileMovementComponent* ProjectileMovement = Projectile->GetProjectileMovement())
			{
				const float ProjectileSpeed = Projectile->GetProjectileDamageProperties().ProjectileSpeed;
				if (ProjectileSpeed <= 0) return EAttackResult::EAR_None;
				
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
					OnAttack();
					return EAttackResult::EAR_Successful;
				}
				
				Projectile->Destroy();
				return EAttackResult::EAR_NoLOS;
			}
				
		}
	}

	return EAttackResult::EAR_None;
}
