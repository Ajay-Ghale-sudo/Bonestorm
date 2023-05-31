// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsEnemyBase.h"

#include "Component/BsHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Hazard/BsHazardBase.h"
#include "Props/Head/BsSeveredHeadBase.h"
#include "Weapon/BsWeaponBase.h"
#include "Weapon/Projectile/BsProjectileBase.h"


// Sets default values
ABsEnemyBase::ABsEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComponent = CreateDefaultSubobject<UBsHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABsEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ABsEnemyBase::Die);
	}
	
}

void ABsEnemyBase::Die()
{
	bIsAlive = false;
	SeverHead();
	TriggerRagdoll();
}

void ABsEnemyBase::TriggerRagdoll()
{	
	if (USkeletalMeshComponent* CurrentMesh = GetMesh())
	{
		CurrentMesh->SetSimulatePhysics(true);
		CurrentMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		CurrentMesh->SetAllBodiesBelowSimulatePhysics(RootBoneName, true, true);
		CurrentMesh->SetCollisionResponseToAllChannels(ECR_Block);
		CurrentMesh->SetPhysicsBlendWeight(RagdollPhysicsBlendWeight);
	}
	if (UCapsuleComponent* CurrentCapsule = GetCapsuleComponent())
	{
		CurrentCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABsEnemyBase::SeverHead()
{
	UWorld* World = GetWorld();
	USkeletalMeshComponent* CurrentMesh = GetMesh();
	if (SeveredHeadClass && World && CurrentMesh) 
	{
		CurrentMesh->HideBoneByName(HeadBoneName, EPhysBodyOp::PBO_None);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		FTransform SpawnTransform = CurrentMesh->GetSocketTransform(HeadBoneName);
		const FQuat InstigatorRotation = GetControlRotation().Quaternion();
		SpawnTransform.SetRotation(InstigatorRotation);
		SpawnTransform.SetScale3D(CurrentMesh->GetComponentScale());
		if (ABsSeveredHeadBase* SeveredHead = World->SpawnActor<ABsSeveredHeadBase>(SeveredHeadClass, SpawnTransform, SpawnParams))
		{
			// TODO: Launch Vector should be determined by the attack.
			const FVector LaunchVector = FVector(0.f, 0.f, 600.f);
			SeveredHead->GetHeadMesh()->AddImpulse(LaunchVector, NAME_None, true);
		}
	}
}

// Called every frame
void ABsEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABsEnemyBase::ReceiveProjectileDamage(const FHitResult& HitResult, ABsProjectileBase* Projectile,
	const float Damage)
{
	if (!Projectile) return;

	// TODO: DamageEvent should come from the projectile
	TakeDamage(Damage, FDamageEvent(), Projectile->GetInstigatorController(), Cast<AActor>(Projectile));
}

void ABsEnemyBase::ReceiveMeleeDamage(const FHitResult& HitResult, ABsWeaponBase* AttackingWeapon, const float Damage)
{
	if (!AttackingWeapon) return;
	
	// TODO: DamageEvent should come from the AttackingWeapon
	TakeDamage(Damage, FDamageEvent(), AttackingWeapon->GetInstigatorController(), AttackingWeapon);
}

void ABsEnemyBase::ReceiveHazardDamage(ABsHazardBase* Hazard, const float Damage)
{
	if (!Hazard) return;
	
	TakeDamage(Damage, FDamageEvent(), Hazard->GetInstigatorController(), Hazard);
}


