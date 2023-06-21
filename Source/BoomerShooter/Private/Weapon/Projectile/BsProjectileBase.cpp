// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/BsProjectileBase.h"
#include "BoomerShooter.h"
#include "Component/BsHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Data/BsDamageType.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
		CheckProjectilePath();
	}
}

bool ABsProjectileBase::CheckProjectilePath()
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
		
		World->LineTraceMultiByChannel(HitResults, StartLocation, EndLocation, ECC_Projectile, CollisionQueryParams);

		bool bHit = false;
		for (auto HitResult : HitResults)
		{
			if (HitResult.GetActor())
			{
				const float TravelDistance = (HitResult.ImpactPoint - GetActorLocation()).Size();
				const float TravelTime = UKismetMathLibrary::SafeDivide(TravelDistance, ProjectileSpeed);
				
				GetWorldTimerManager().SetTimer(ProjectileDamageProperties.ImpactTimerHandle, this, &ABsProjectileBase::OnImpact, TravelTime, false);
				ApplyDamageToActor(HitResult.GetActor());
				SetProjectileCollision(ECollisionEnabled::NoCollision);
				bHit = true;
				break;
			}
		}
		return bHit;
	}

	return false;
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
	if (Owner)
	{
		TArray<AActor*> ActorsToIgnore;
		Owner->GetAllChildActors(ActorsToIgnore);
		ActorsToIgnore.Add(Owner);
		ProjectileCollision->MoveIgnoreActors = ActorsToIgnore;
	}
}

void ABsProjectileBase::ApplyDamageToActor(AActor* OtherActor)
{
	if (OtherActor && OtherActor != GetOwner())
	{
		OtherActor->TakeDamage(
				ProjectileDamageProperties.ProjectileDamage,
				FDamageEvent(ProjectileDamageProperties.ProjectileDamageType),
				GetInstigatorController(),
				this
		);
	}
}

void ABsProjectileBase::OnImpact()
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

}

void ABsProjectileBase::OnProjectileOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnProjectileOverlapInternal(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ABsProjectileBase::OnProjectileOverlapInternal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner() && ProjectileMovement)
	{
		OnImpact();
		// TODO: Handle penetration.
		ApplyDamageToActor(OtherActor);
		SetLifeSpan(ProjectileDamageProperties.ProjectileLifeTime);
	}
}

void ABsProjectileBase::ProjectileParried(AActor* DamageCauser)
{
	if (!bParried)
	{
		bParried = true;
		const AActor* ProjectileOwner = GetOwner();
		if (DamageCauser && ProjectileOwner)
		{
			SetOwner(DamageCauser);
			UpdateMoveActorIgnore();
			const FVector TargetLocation = ProjectileOwner->GetActorLocation();
			const FVector InitLocation = GetActorLocation();
			const FVector VelocityDirection = UKismetMathLibrary::FindLookAtRotation(InitLocation, TargetLocation).Vector();
			if (ProjectileMovement)
			{
				SetActorHiddenInGame(false);
				ProjectileMovement->Velocity = ProjectileMovement->MaxSpeed * VelocityDirection;
				SetActorLocation(InitLocation + VelocityDirection * 100.f);
				SetProjectileCollision(ECollisionEnabled::QueryAndPhysics);
				ProjectileMovement->UpdateComponentVelocity();
				SetLifeSpan(0);
			}
		}
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