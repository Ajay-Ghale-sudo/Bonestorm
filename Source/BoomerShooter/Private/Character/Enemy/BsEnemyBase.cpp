// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsEnemyBase.h"
#include "Component/BsHealthComponent.h"
#include "Component/Audio/BsEnemyAudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Data/AttackResult.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Hazard/BsHazardBase.h"
#include "Interfaces/Decapitator.h"
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
	EnemyAudioComponent = CreateDefaultSubobject<UBsEnemyAudioComponent>(TEXT("EnemyAudioComponent"));
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
		HealthComponent->OnLowHealth.AddDynamic(this, &ABsEnemyBase::IndicateLowHealth);
		HealthComponent->OnExplosionHit.AddUObject(this, &ABsEnemyBase::ExplosionLaunch);
	}

	if (const UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		PreHitStunMaxWalkSpeed = MovementComponent->MaxWalkSpeed;
	}
	
}

// Called every frame
void ABsEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABsEnemyBase::Die()
{
	// Do nothing if we're already dead.
	if (!bIsAlive) return;
	
	bIsAlive = false;
	SeverHead();
	TriggerRagdoll();
	ClearOverlayMaterial();
	HitStunTimerHandle.Invalidate();
	SetMeleeHitBoxEnabled(false);
	OnThisEnemyDeath.Broadcast(this);
	OnDeath.Broadcast();
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
			if (HealthComponent)
			{
				if (IDecapitator* Decapitator = Cast<IDecapitator>(HealthComponent->GetLastDamagedBy()))
				{
					Decapitator->DecapitatedActor(SeveredHead);
				}	
			}
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

void ABsEnemyBase::ExplosionLaunch(FVector LaunchVector)
{
	LaunchCharacter(LaunchVector, true, true);
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
	ApplyHitStun(HitStunDuration);
}

void ABsEnemyBase::ApplyHitStun(float Duration)
{
	if (bHitStunned)
	{
		if (Duration <= HitStunDuration)
		{
			return;
		}
	}
	PlayMontage(HitStunMontage);
	
	if (UCharacterMovementComponent* CMovement = GetCharacterMovement())
	{
		bHitStunned = true;
		CMovement->MaxWalkSpeed = 0.f;
		
		GetWorldTimerManager().SetTimer(
			HitStunTimerHandle,
			this,
			&ABsEnemyBase::EndHitStun,
			Duration, 
			false
		);
	}
	OnHitStun.Broadcast();
}

void ABsEnemyBase::EndHitStun()
{
	if (UCharacterMovementComponent* CMovement = GetCharacterMovement())
	{
		CMovement->MaxWalkSpeed = PreHitStunMaxWalkSpeed;
	}
	ClearOverlayMaterial();
	bHitStunned = false;
}

void ABsEnemyBase::IndicateLowHealth()
{
	if (USkeletalMeshComponent* CurrentMesh = GetMesh())
	{
		CurrentMesh->SetOverlayMaterial(LowHealthMaterial);
		ApplyHitStun(LowHealthHitStunDuration);
	}
}

void ABsEnemyBase::ClearOverlayMaterial()
{
	if (USkeletalMeshComponent* CurrentMesh = GetMesh())
	{
		CurrentMesh->SetOverlayMaterial(nullptr);
	}
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
	return bIsAlive && bCanAttack && !bHitStunned;
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
