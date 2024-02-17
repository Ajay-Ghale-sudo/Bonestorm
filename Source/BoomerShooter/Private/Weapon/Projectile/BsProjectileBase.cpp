// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/BsProjectileBase.h"
#include "BoomerShooter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Component/BsHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Data/BsDamageType.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABsProjectileBase::ABsProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(ProjectileMesh);
	
	ProjectileCollision = CreateDefaultSubobject<USphereComponent>("ProjectileCollision");
	ProjectileCollision->SetupAttachment(ProjectileMesh);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollision->SetCollisionObjectType(ECC_Projectile);
	ProjectileCollision->bReturnMaterialOnMove = true;
}

// Called when the game starts or when spawned
void ABsProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileCollision)
	{
		ProjectileCollision->OnComponentHit.AddDynamic(this, &ABsProjectileBase::OnProjectileHit);
		ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ABsProjectileBase::OnProjectileOverlap);
		UpdateMoveActorIgnore();
	}

	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = ProjectileDamageProperties.ProjectileSpeed;
	}

	if (ProjectileDamageProperties.bSupportsCheckProjectilePath)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ABsProjectileBase::CheckProjectilePath);
	}
	SetLifeSpan(ProjectileDamageProperties.ProjectileLifeTime);
}

void ABsProjectileBase::CheckProjectilePath()
{
	if (UWorld* World = GetWorld())
	{
		const float ProjectileSpeed = ProjectileMovement ? ProjectileMovement->MaxSpeed : ProjectileDamageProperties.ProjectileSpeed;
		// TODO: Magic Number should be determined from the projectile speed and current avg frame time.
		float Distance = ProjectileSpeed * 0.16f; // How far the projectile will go. 
		const FVector StartLocation = GetActorLocation();
		const FVector EndLocation = GetActorLocation() + (GetActorForwardVector() * Distance);

		TArray<FHitResult> HitResults;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);
		CollisionQueryParams.AddIgnoredActor(GetOwner());
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.bReturnPhysicalMaterial = true;
		
		World->LineTraceMultiByChannel(HitResults, StartLocation, EndLocation, ECC_Projectile, CollisionQueryParams);

		for (auto HitResult : HitResults)
		{
			if (HitResult.GetActor())
			{
				const float TravelDistance = (HitResult.ImpactPoint - GetActorLocation()).Size();
				float TravelTime = UKismetMathLibrary::SafeDivide(TravelDistance, ProjectileSpeed);
				
				LastHitResult = HitResult;
				if (TravelTime <= 0.f)
				{
					GetWorldTimerManager().SetTimerForNextTick(this, &ABsProjectileBase::Impact);
					GetWorldTimerManager().SetTimerForNextTick(this, &ABsProjectileBase::ResolveImpact);
				}
				else
				{
					GetWorldTimerManager().SetTimer(ProjectileDamageProperties.ResolveImpactTimerHandle, this, &ABsProjectileBase::ResolveImpact, TravelTime, false);
					GetWorldTimerManager().SetTimer(ProjectileDamageProperties.ImpactTimerHandle, this, &ABsProjectileBase::Impact, TravelTime, false);
				}
				ApplyDamageToActor(HitResult.GetActor());
				SetProjectileCollision(ECollisionEnabled::NoCollision);
				return;
			}
		}
	}
	GetWorldTimerManager().SetTimerForNextTick(this, &ABsProjectileBase::CheckProjectilePath);
}

void ABsProjectileBase::ResolveImpact()
{
	if (DamageDealt > 0.f)
	{
		OnDealtDamage.Broadcast(LastHitResult);
		OnDamagedActor.Broadcast(LastHitResult.GetActor());
	}
}

void ABsProjectileBase::SetDamageType(TSubclassOf<UDamageType> DamageType)
{
	ProjectileDamageProperties.ProjectileDamageType = DamageType;
}

void ABsProjectileBase::SetProjectileCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (ProjectileCollision)
	{
		ProjectileCollision->SetCollisionEnabled(CollisionEnabled);
	}
}

void ABsProjectileBase::UpdateMoveActorIgnore()
{
	if (Owner && ProjectileCollision)
	{
		TArray<AActor*> ActorsToIgnore;
		Owner->GetAllChildActors(ActorsToIgnore);
		ActorsToIgnore.Add(Owner);
		ProjectileCollision->MoveIgnoreActors = ActorsToIgnore;
	}
}

float ABsProjectileBase::ApplyDamageToActor(AActor* OtherActor)
{
	float DamageApplied = 0.f;
	if (OtherActor && OtherActor != GetOwner())
	{
		DamageApplied = OtherActor->TakeDamage(
				ProjectileDamageProperties.ProjectileDamage,
				FDamageEvent(ProjectileDamageProperties.ProjectileDamageType),
				GetInstigatorController(),
				this
		);
	}

	DamageDealt += DamageApplied;
	return DamageApplied;
}

void ABsProjectileBase::Impact()
{
	SetActorHiddenInGame(true);
	ProjectileMovement->StopMovementImmediately();
	SetProjectileCollision(ECollisionEnabled::NoCollision);
}

void ABsProjectileBase::OnProjectileHit_Implementation(UPrimitiveComponent* OnComponentHit, AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnProjectileHitInternal(OnComponentHit, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ABsProjectileBase::OnProjectileHitInternal(UPrimitiveComponent* OnComponentHit, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	LastHitResult = Hit;
}

void ABsProjectileBase::OnProjectileOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LastHitResult = SweepResult;
	OnProjectileOverlapInternal(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ABsProjectileBase::OnProjectileOverlapInternal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LastHitResult = SweepResult;
	// If we don't have a sweep, we'll need to do a line trace to find the hit result.
	if (!bFromSweep)
	{
		if (const UWorld* World = GetWorld())
		{
			FCollisionQueryParams CollisionParameters;
			CollisionParameters.AddIgnoredActor(this);
			CollisionParameters.AddIgnoredActor(GetOwner());
			CollisionParameters.bReturnPhysicalMaterial = true;
			CollisionParameters.bTraceComplex = true;
			
			TArray<FHitResult> HitResults;
			// TODO: Should this be an Overlap test instead?
			World->LineTraceMultiByChannel(
				HitResults,
				GetActorLocation(),
				GetActorLocation() + (GetActorForwardVector() * 500.f),
				ECC_Projectile,
				CollisionParameters
			);

			if (!HitResults.IsEmpty())
			{
				LastHitResult = HitResults[0];
			}
		}
	}
	
	if (OtherActor && OtherActor != GetOwner())
	{
		Impact();
		ApplyDamageToActor(OtherActor);
		ResolveImpact();
		// TODO: Handle penetration.
		SetLifeSpan(ProjectileDamageProperties.ProjectileLifeTime);
	}
}

void ABsProjectileBase::ProjectileParried(AActor* DamageCauser)
{
	if (!bParried)
	{
		bParried = true;
		const AActor* ProjectileOwner = GetOwner();
		if (DamageCauser)
		{
			if (!ProjectileOwner)
			{
				// TODO: Handle case where projectile doesn't have an owner. Destroy or reflect back?
				return;
			}
			Impact();
			InitParryFX();
			SetOwner(DamageCauser);
			UpdateMoveActorIgnore();
			ProjectileDamageProperties.ProjectileDamageType = UBsDamageType::StaticClass();
			
			const FVector TargetLocation = ProjectileOwner->GetActorLocation();
			const FVector InitLocation = GetActorLocation();
			const FVector VelocityDirection = UKismetMathLibrary::FindLookAtRotation(InitLocation, TargetLocation).Vector();
			
			if (ProjectileMovement)
			{
				ProjectileMovement->MaxSpeed = ProjectileMovement->MaxSpeed * ProjectileDamageProperties.ParrySpeedModifier;
				ProjectileMovement->Velocity = ProjectileMovement->MaxSpeed * VelocityDirection;
				ProjectileMovement->HomingTargetComponent = ProjectileOwner->GetRootComponent();
				ProjectileMovement->bIsHomingProjectile = true;
				ProjectileMovement->HomingAccelerationMagnitude = ProjectileMovement->MaxSpeed;
				ProjectileMovement->UpdateComponentVelocity();
			}
			
			SetActorHiddenInGame(false);
			SetLifeSpan(0);
			SetProjectileCollision(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void ABsProjectileBase::InitParryFX()
{
	if (ParryTrailFX)
	{
		ParryTrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(ParryTrailFX,
			ProjectileMesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true);
	}
}

void ABsProjectileBase::ActivateParryFX()
{
	if (ParryTrailComponent)
	{
		ParryTrailComponent->Activate(true);
	}
}

void ABsProjectileBase::DeactivateParryFX()
{
	if (ParryTrailComponent)
	{
		ParryTrailComponent->SetActive(false);
	}
}

float ABsProjectileBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                    AActor* DamageCauser)
{
	if (DamageCauser && DamageCauser->IsA(ABsProjectileBase::StaticClass()))
	{
		return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	} 
	
	if (DamageEvent.DamageTypeClass == UBsParryDamageType::StaticClass() && !bParried)
	{
		ProjectileParried(DamageCauser);
	}
	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}
