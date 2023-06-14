// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsEnemyBase.h"
#include "Component/BsHealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Data/AttackResult.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	BindMeleeHitBox();
	SetMeleeHitBoxEnabled(false);
	
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ABsEnemyBase::Die);
		HealthComponent->OnTookDamage.AddDynamic(this, &ABsEnemyBase::StartHitStun);
	}

	if (const UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		PreHitStunMovementMode = MovementComponent->MovementMode;
	}
	
}

// Called every frame
void ABsEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABsEnemyBase::Die()
{
	bIsAlive = false;
	SeverHead();
	TriggerRagdoll();
	SetMeleeHitBoxEnabled(false);
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

void ABsEnemyBase::EnableAttack()
{
	bCanAttack = true;
}

void ABsEnemyBase::Attack()
{
}

void ABsEnemyBase::OnAttack()
{
	bCanAttack = false;
	GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&ABsEnemyBase::EnableAttack,
		AttackCooldown,
		false
	);
}

void ABsEnemyBase::BindMeleeHitBox()
{
	// Nothing by default
}

void ABsEnemyBase::SetMeleeHitBoxEnabled(bool bEnabled)
{
	if (MeleeHitBox)
	{
		MeleeHitBox->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void ABsEnemyBase::StartHitStun()
{
	if (bHitStunned) return;
	
	PlayMontage(HitStunMontage);
	
	if (UCharacterMovementComponent* CMovement = GetCharacterMovement())
	{
		bHitStunned = true;
		PreHitStunMovementMode = CMovement->MovementMode;
		CMovement->DisableMovement();
		
		GetWorldTimerManager().SetTimer(
			HitStunTimerHandle,
			this,
			&ABsEnemyBase::EndHitStun,
			HitStunDuration, // TODO: This could be determined by the attack or the montage played.
			false
		);
	}
}

void ABsEnemyBase::EndHitStun()
{
	if (UCharacterMovementComponent* CMovement = GetCharacterMovement())
	{
		CMovement->SetMovementMode(PreHitStunMovementMode);
	}

	bHitStunned = false;
}


void ABsEnemyBase::PlayMontage(UAnimMontage* MontageToPlay) const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(MontageToPlay);
	}
}

bool ABsEnemyBase::CanAttack() const
{
	return bIsAlive && bCanAttack;
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

EAttackResult ABsEnemyBase::MeleeAttack()
{
	return EAttackResult::EAR_None;
}

bool ABsEnemyBase::CanMeleeAttackTarget(AActor* Target)
{
	if (!Target || !CanAttack()) return false;

	if (FVector::Dist(GetActorLocation(), Target->GetActorLocation()) <= MeleeAttackRange)
	{
		// TODO: Is the target in front of the enemy?
		// TODO: Do we have LOS?
		return true;
	}

	return false;
}
