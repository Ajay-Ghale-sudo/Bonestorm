// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BsGrappleHookComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Weapon/Projectile/BsGrappleProjectile.h"


// Sets default values for this component's properties
UBsGrappleHookComponent::UBsGrappleHookComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UBsGrappleHookComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

ABsGrappleProjectile* UBsGrappleHookComponent::FireGrappleHook(const FVector& StartLocation, const FVector& Direction)
{
	if (!GrappleHookProperties.GrappleProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("GrappleProjectileClass is not set!"))
		return nullptr;
	}

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.Instigator = Cast<APawn>(GetOwner());
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(StartLocation);
		SpawnTransform.SetRotation(Direction.Rotation().Quaternion());

		if (ABsGrappleProjectile* GrappleHook = Cast<ABsGrappleProjectile>(World->SpawnActor(GrappleHookProperties.GrappleProjectileClass, &SpawnTransform, SpawnParameters)))
		{
			GrappleHookProperties.GrappleProjectile = GrappleHook;
			GrappleHook->OnGrappleComponentAttached.AddDynamic(this, &UBsGrappleHookComponent::GrappleHookAttached);
			GrappleHook->OnGrappleComponentDetached.AddDynamic(this, &UBsGrappleHookComponent::GrappleHookDetached);

			InitFX();
			return GrappleHook;
		}
	}

	return nullptr;
}



void UBsGrappleHookComponent::DetachGrappleHook()
{
	if (!GrappleHookProperties.bIsAttached) return; // If we are not attached, return
	
	GrappleHookProperties.GrapplePullTimerHandle.Invalidate();
	GrappleHookProperties.bIsAttached = false;
	if (GrappleHookProperties.GrappleProjectile)
	{
		GrappleHookProperties.GrappleProjectile->Detach();
		GrappleHookProperties.GrappleProjectile = nullptr;
		return;
	}
	
	OnGrappleHookDetached.Broadcast();
}

void UBsGrappleHookComponent::PullOwnerToGrappleProjectile()
{
	GrappleHookProperties.GrapplePullTimerHandle.Invalidate();

	// If we cannot reach the grapple point, detach and return
	if (!OwnerCanReachGrappleProjectile())
	{
		if (GrappleHookProperties.GrappleProjectile) GrappleHookProperties.GrappleProjectile->Detach();
		return;
	}
	
	
	if (EffectedCharacter)
	{
		FVector Direction = GrappleHookProperties.GrappleProjectile->GetActorLocation() - EffectedCharacter->GetActorLocation();
		Direction.Normalize();
		
		// TODO: Review ways that still allow character movement influence
		FVector Movement = Direction * GrappleHookProperties.PullForce;
		OnGrappleHookPull.Broadcast(Movement);
		GrappleHookProperties.GrapplePullTimerHandle =
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UBsGrappleHookComponent::PullOwnerToGrappleProjectile);
	}

	GrappleHookProperties.AttachTime += GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f;
}

void UBsGrappleHookComponent::PullOwnerToLocation()
{
	GrappleHookProperties.GrapplePullTimerHandle.Invalidate();
	if (EffectedCharacter)
	{
		if (!OwnerCanReachGrapplePoint())
		{
			DetachGrappleHook();
			return;
		}
		
		FVector Location = GrappleHookProperties.GrapplePointLocation;
		float Distance = FVector::Dist(Location, EffectedCharacter->GetActorLocation());
		if (Distance <= GrappleHookProperties.DistanceCompletionThreshold || Distance >= GrappleHookProperties.MaxDistance)
		{
			GrappleHookDetached();
			return;
		}
		
		FVector Direction = Location - EffectedCharacter->GetActorLocation();
		Direction.Normalize();

		FVector Movement = Direction * GrappleHookProperties.PullForce;
		OnGrappleHookPull.Broadcast(Movement);
		GrappleHookProperties.GrapplePullTimerHandle =
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UBsGrappleHookComponent::PullOwnerTick);

		GrappleHookProperties.AttachTime += GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f;
	}
}

void UBsGrappleHookComponent::PullOwnerTick()
{
	if (GrappleHookProperties.bIsAttached)
	{
		PullOwnerToLocation();
	}
}

void UBsGrappleHookComponent::SetEffectedCharacter(ACharacter* Character)
{
	EffectedCharacter = Character;
}

void UBsGrappleHookComponent::SetGrappleFXAttachPoint(USceneComponent* AttachPoint)
{
	GrappleHookProperties.GrappleFXComponentAttachPoint = AttachPoint;
}

void UBsGrappleHookComponent::AttachToGrapplePoint(UBsGrapplePointComponent* GrapplePoint)
{
	if (!GrapplePoint) return;
	GrappleHookAttached();
	GrappleHookProperties.GrapplePointLocation = GrapplePoint->GetComponentLocation();
	GrappleHookProperties.AttachedGrapplePoint = GrapplePoint;
}


void UBsGrappleHookComponent::GrappleHookAttached()
{
	OnGrappleHookAttached.Broadcast();
	// Set the Attached time. We set it to -GetWorld()->GetDeltaSeconds() so that the first tick will be 0
	GrappleHookProperties.AttachTime =  GetWorld() ? -GetWorld()->GetDeltaSeconds() : 0.f;
	GrappleHookProperties.bIsAttached = true;
}

void UBsGrappleHookComponent::GrappleHookDetached()
{
	GrappleHookProperties.GrapplePullTimerHandle.Invalidate();
	GrappleHookProperties.bIsAttached = false;
	GrappleHookProperties.GrapplePointLocation = FVector::ZeroVector;
	OnGrappleHookDetached.Broadcast();
}

bool UBsGrappleHookComponent::OwnerCanSeeGrappleProjectile() const
{
	if (!GrappleHookProperties.GrappleProjectile)
	{
		UE_LOG(LogTemp, Log, TEXT("Grapple projectile is null"))
		return false;
	}
	if (!GetOwner()) return false;
	
	// Can we still see the grapple point?
	if (const UWorld* World = GetWorld())
	{
		FHitResult HitResult;
		FCollisionQueryParams CollisionParameters;
		CollisionParameters.AddIgnoredActor(GetOwner());
		TArray<AActor*> AttachedActors;
		GetOwner()->GetAttachedActors(AttachedActors);
		CollisionParameters.AddIgnoredActors(AttachedActors);
		
		if (AActor* GrapplePoint = GrappleHookProperties.GrappleProjectile->GetGrapplePointActor())
		{
			CollisionParameters.AddIgnoredActor(GrapplePoint);
		}
		
		World->LineTraceSingleByChannel(
			HitResult,
			GetOwner()->GetActorLocation(),
			GrappleHookProperties.GrappleProjectile->GetActorLocation(),
			ECollisionChannel::ECC_Visibility,
			CollisionParameters
		);
		
		if (HitResult.bBlockingHit)
		{
			if (HitResult.GetActor() == GrappleHookProperties.GrappleProjectile)
			{
				return true;
			}
		}
	}

	return false;
}

bool UBsGrappleHookComponent::OwnerCanSeeGrapplePoint() const
{
	if (!GetOwner() || !EffectedCharacter || !GrappleHookProperties.AttachedGrapplePoint) return false;
	
	// Can we still see the grapple point?
	if (const UWorld* World = GetWorld())
	{
		FHitResult HitResult;
		FCollisionQueryParams CollisionParameters;
		TArray<AActor*> AttachedActors;
		
		GetOwner()->GetAttachedActors(AttachedActors);
		CollisionParameters.AddIgnoredActors(AttachedActors);
		CollisionParameters.AddIgnoredActor(GetOwner());
		CollisionParameters.AddIgnoredActor(GrappleHookProperties.AttachedGrapplePoint->GetOwner());
		CollisionParameters.AddIgnoredActor(EffectedCharacter);
		
		World->LineTraceSingleByChannel(
			HitResult,
			EffectedCharacter->GetActorLocation(),
			GrappleHookProperties.GrapplePointLocation,
			ECollisionChannel::ECC_Visibility,
			CollisionParameters
		);

		return !HitResult.bBlockingHit;
	}

	return false;
}

bool UBsGrappleHookComponent::OwnerCanReachGrappleProjectile() const
{
	// If the owner no longer exists, detach the grapple hook and return
	if (!GetOwner() || !GrappleHookProperties.GrappleProjectile)
	{
		return false;
	}
	

	if (GrappleHookProperties.AttachTime >= GrappleHookProperties.MaxAttachTime)
	{
		return false;
	}

	// Are we close enough to the grapple point?
	const float DistanceRemaining = FVector::Dist(GetOwner()->GetActorLocation(), GrappleHookProperties.GrappleProjectile->GetActorLocation());
	if (DistanceRemaining <= GrappleHookProperties.DistanceCompletionThreshold)
	{
		return false;
	}

	return true;
}

bool UBsGrappleHookComponent::OwnerCanReachGrapplePoint() const
{
	if (!GetOwner() || !OwnerCanSeeGrapplePoint()) return false;

	if (GrappleHookProperties.AttachTime >= GrappleHookProperties.MaxAttachTime)
	{
		return false;
	}

	return true;
}

void UBsGrappleHookComponent::InitFX()
{
	if (GrappleHookProperties.GrappleFX && GrappleHookProperties.GrappleProjectile)
	{
		GrappleHookProperties.GrappleFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			GrappleHookProperties.GrappleFX,
			GrappleHookProperties.GrappleProjectile->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);

		UpdateGrappleFX();
	}
}

void UBsGrappleHookComponent::UpdateGrappleFX()
{
	GrappleHookProperties.GrapplePullTimerHandle.Invalidate();
	if (GrappleHookProperties.GrappleFXComponent && GrappleHookProperties.GrappleProjectile && EffectedCharacter)
	{
		if (!GrappleHookProperties.GrappleFXComponent->IsActive())
		{
			GrappleHookProperties.GrappleFXComponent->Activate(true);
		}

		if (GrappleHookProperties.GrappleFXComponentAttachPoint)
		{
			GrappleHookProperties.GrappleFXComponent->SetVectorParameter(FName("StartPosition"), GrappleHookProperties.GrappleFXComponentAttachPoint->GetSocketLocation(FName("BladeSocket")));
			GrappleHookProperties.GrappleFXUpdateTimerHandle =
				GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UBsGrappleHookComponent::UpdateGrappleFX);
		}
	}
}
