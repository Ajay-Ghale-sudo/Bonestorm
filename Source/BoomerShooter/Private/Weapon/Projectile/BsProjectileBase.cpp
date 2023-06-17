// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/BsProjectileBase.h"

#include "BoomerShooter.h"
#include "Component/BsHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"


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
	}

	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = ProjectileDamageProperties.ProjectileSpeed;
	}
}

void ABsProjectileBase::SetDamageType(TSubclassOf<UDamageType> DamageType)
{
	ProjectileDamageProperties.ProjectileDamageType = DamageType;
}

void ABsProjectileBase::StopMovementAndDisableCollision()
{
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
	}

	SetActorEnableCollision(false);
}

void ABsProjectileBase::OnImpact()
{
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
	if (OtherActor && OtherActor != GetOwner())
	{
		// TODO: Handle penetration.
		OtherActor->TakeDamage(
			ProjectileDamageProperties.ProjectileDamage,
			FDamageEvent(ProjectileDamageProperties.ProjectileDamageType),
			GetInstigatorController(),
			this
		);
		OnImpact();
		StopMovementAndDisableCollision();
		Destroy();
	}
}