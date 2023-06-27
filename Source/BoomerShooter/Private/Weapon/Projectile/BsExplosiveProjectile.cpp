// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/BsExplosiveProjectile.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Data/BsDamageType.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ABsExplosiveProjectile::ABsExplosiveProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ExplosionForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("ExplosionForceComponent"));
	ExplosionForceComponent->SetupAttachment(ProjectileMesh);
	ExplosionForceComponent->Radius = ExplosionRadius;
	ExplosionForceComponent->ImpulseStrength = ExplosionImpulse;
	ExplosionForceComponent->bImpulseVelChange  = true;

	ProjectileDamageProperties.ProjectileDamageType = UBsExplosionDamageType::StaticClass();
}

// Called when the game starts or when spawned
void ABsExplosiveProjectile::BeginPlay()
{
	Super::BeginPlay();
	InitFX();
}

void ABsExplosiveProjectile::InitFX()
{
	if (ProjectileFX)
	{
		ProjectileFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ProjectileFX,
			ProjectileMesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true,
			false,
			ENCPoolMethod::AutoRelease,
			true
		);
	}

	if (ImpactFX)
	{
		ImpactFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ImpactFX,
			ProjectileMesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true,
			false,
			ENCPoolMethod::AutoRelease,
			true
		);
	}
}

void ABsExplosiveProjectile::ShowProjectileFX() const
{
	if (ProjectileFXComponent)
	{
		ProjectileFXComponent->Activate(true);
	}
}

void ABsExplosiveProjectile::ShowImpactFX() const
{
	if (ImpactFXComponent)
	{
		ImpactFXComponent->Activate(true);
	}
}

void ABsExplosiveProjectile::CreateExplosion()
{

	const TSubclassOf<UDamageType> DamageType = ProjectileDamageProperties.ProjectileDamageType ?
		ProjectileDamageProperties.ProjectileDamageType : UBsExplosionDamageType::StaticClass();
	
	UGameplayStatics::ApplyRadialDamage(
		this,
		ProjectileDamageProperties.ProjectileDamage,
		GetActorLocation(),
		ExplosionRadius,
		DamageType,
		{},
		this,
		nullptr,
		true,
		ECC_Visibility
	);
	ShowImpactFX();	
}

void ABsExplosiveProjectile::Impact()
{
	Super::Impact();
	CreateExplosion();
}