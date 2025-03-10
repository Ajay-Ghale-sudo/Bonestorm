#include "Weapon/Scythe/BsScythe.h"
#include "BoomerShooter.h"
#include "Component/BsGrappleHookComponent.h"
#include "Components/BoxComponent.h"
#include "Data/BsDamageType.h"
#include "Engine/DamageEvents.h"
#include "Interfaces/ReceiveDamage.h"
#include "Props/Head/BsSeveredHeadBase.h"
#include "Weapon/Projectile/BsGrappleProjectile.h"
#include "Weapon/Projectile/BsProjectileBase.h"

ABsScythe::ABsScythe()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MeleeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeCollision"));
	MeleeCollision->SetupAttachment(WeaponMesh, FName("BladeSocket"));
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeleeCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

	ScytheCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ScytheCollision"));
	ScytheCollision->SetupAttachment(WeaponMesh);
	ScytheCollision->SetCollisionObjectType(ECC_Scythe);
	ScytheCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	GrappleHookComponent = CreateDefaultSubobject<UBsGrappleHookComponent>(TEXT("GrappleHookComponent"));
	
}

void ABsScythe::BeginPlay()
{
	Super::BeginPlay();
	
	if (ScytheCollision)
	{
		ScytheCollision->OnComponentBeginOverlap.AddDynamic(this, &ABsScythe::OnScytheOverlap);
	}

	if (MeleeCollision)
	{
		MeleeCollision->OnComponentBeginOverlap.AddDynamic(this, &ABsScythe::OnMeleeOverlap);
	}

	if (GrappleHookComponent)
	{
		GrappleHookComponent->SetGrappleFXAttachPoint(WeaponMesh);
		GrappleHookComponent->OnGrappleHookDetached.AddDynamic(this, &ABsScythe::OnGrappleHookDetached);
	}	
}

void ABsScythe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ThrowTick(DeltaTime);
}

void ABsScythe::ThrowTick(float DeltaTime)
{
	if (!bThrown || !Owner) return;

	const float DistanceToOwner = FVector::Dist(GetActorLocation(), Owner->GetActorLocation());
	if (bAttachedToGrapplePoint)
	{
		if (GrappleHookComponent && DistanceToOwner >= ThrowDistance)
		{
			GrappleHookComponent->DetachGrappleHook();
		}
		return;
	}

	FVector Direction = GetActorForwardVector();
	if (bReturningToOwner)
	{
		if (DistanceToOwner < ReturnDistanceThreshold)
		{
			bThrown = false;
			bReturningToOwner = false;
			OnWeaponCaught.Broadcast();
			ClearMontage(ThrowMontage);
			OnWeaponThrowEnd.Broadcast();
			bCanAttack = true;
			return;			
		}
		Direction = Owner->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
	}
	
	SetActorLocation(GetActorLocation() + (Direction * DeltaTime * ThrowSpeed * (bReturningToOwner ? 2.f : 1.f)));
	const float DistanceFromStart = FVector::Dist(GetActorLocation(), ThrowStartLocation);
	if (DistanceFromStart >= ThrowDistance)
	{
		bReturningToOwner = true;
	}
}

bool ABsScythe::CanAttack() const
{
	if (bThrown)
	{
		return false;
	}
	
	if (IsMeleeMode())
	{
		return bCanAttack;
	}
	
	if (IsRangedMode())
	{
		return bCanAttack && RangedConfig.bCanFire;
	}
	
	return true;
}

bool ABsScythe::CanMeleeAttack() const
{
	return !bThrown && bCanAttack;
}

float ABsScythe::BlockIncomingDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Damage;
	if (BlockConfig.bParrying)
	{
		if (AttachedSeveredHead)
		{
			AttachedSeveredHead->ParryDamage(ActualDamage);
		}
		OnScytheParryEvent.Broadcast(ActualDamage);
		OnWeaponParry.Broadcast();
		
		ActualDamage = 0.f;
		if (DamageCauser)
		{
			DamageCauser->TakeDamage(Damage, FDamageEvent(UBsParryDamageType::StaticClass()), GetInstigatorController(), GetOwner());
		}
		
		return ActualDamage;
	}
	if (BlockConfig.bBlocking)
	{
		ActualDamage = AttachedSeveredHead ?
			AttachedSeveredHead->BlockDamage(ActualDamage) : ActualDamage * BlockConfig.BlockingDamageReduction;
		
		OnScytheBlockEvent.Broadcast(ActualDamage);
		OnWeaponBlock.Broadcast();
		return ActualDamage;
	}
	return Super::BlockIncomingDamage(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
}

void ABsScythe::StartBlock()
{
	if (bThrown) return;
	PlayMontage(BlockMontage);
	StartParry();
	BlockConfig.bBlocking = true;
}

void ABsScythe::StopBlock()
{
	ClearMontage(BlockMontage);
	BlockConfig.bBlocking = false;
	StopParry();
}

void ABsScythe::StartParry()
{
	if (BlockConfig.bCanParry)
	{
		BlockConfig.bParrying = true;
		BlockConfig.bCanParry = false;
		GetWorldTimerManager().SetTimer(BlockConfig.ParryTimerHandle, this, &ABsScythe::StopParry, BlockConfig.ParryDuration, false);
	}
}

void ABsScythe::StopParry()
{
	BlockConfig.bParrying = false;
	BlockConfig.ParryTimerHandle.Invalidate();
	GetWorldTimerManager().SetTimer(BlockConfig.ParryCooldownHandle, this, &ABsScythe::EnableParry, BlockConfig.ParryCooldown);
}

void ABsScythe::EnableParry()
{
	BlockConfig.bCanParry = true;
}

void ABsScythe::EnableRangedFire()
{
	RangedConfig.bCanFire = true;
	ShotgunConfig.bCanFire = true;
}

void ABsScythe::MeleeAttackFinished()
{
	bIsAttacking = false;
	bCanAttack = true;
	StopParry();

	if (MeleeCollision)
	{
		MeleeHitActors.Empty();
	}
}

void ABsScythe::Fire()
{
	if (!CanAttack()) return;

	switch (WeaponMode)
	{
		case EScytheWeaponMode::ESWM_Melee:
			MeleeAttack();
			break;
		case EScytheWeaponMode::ESWM_Shotgun:
		case EScytheWeaponMode::ESWM_Range:
			RangeAttack();
		default:
			break;
	}
}

void ABsScythe::SecondaryFire()
{
	MeleeAttack();
}

void ABsScythe::RangeAttack()
{
	// Spawn Projectile
	UWorld* World = GetWorld();
	auto& [FireRate, ProjectileClass, bCanFire, FireRateTimerHandle] = WeaponMode == EScytheWeaponMode::ESWM_Shotgun ? ShotgunConfig : RangedConfig;
	if (ProjectileClass && World && bCanFire)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = GetInstigator();

		const int32 MaxProjectiles = WeaponMode == EScytheWeaponMode::ESWM_Shotgun ? ShotgunConfig.PelletCount : 1;
		for (int32 SpawnedProjectiles = 0; SpawnedProjectiles < MaxProjectiles; ++SpawnedProjectiles)
		{
			ABsProjectileBase* Projectile = nullptr;
			const FTransform SpawnTransform = GetProjectileSpawnTransform();
			
			if (AttachedSeveredHead)
			{
				// TODO: Attached Severed Head mechanics
				// Add effect to Projectile?
				// Change the projectile that is spawned?
				Projectile = AttachedSeveredHead->CreateProjectile(ProjectileClass, SpawnTransform, SpawnParams);
			}
			if (!Projectile)
			{
				Projectile = Cast<ABsProjectileBase>(World->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams));
			}
			
			if (Projectile)
			{
				Projectile->SetOwner(GetOwner());
				Projectile->SetInstigator(GetInstigator());
				OnCreateProjectileEvent.Broadcast(Projectile);
			}
		}

		PlayMontage(RangedAttackMontage);
		OnRangedAttack();
		OnWeaponFire.Broadcast();
		bCanFire = false;

		World->GetTimerManager()
			.SetTimer(
				FireRateTimerHandle,
				this,
				&ABsScythe::EnableRangedFire,
				FireRate,
				false
		);
	}
}

void ABsScythe::MeleeAttack()
{
	if (!CanMeleeAttack()) return;

	OnMeleeAttack();
	OnWeaponMeleeAttack.Broadcast();
	bCanAttack = false;
	bIsAttacking = true;
	PlayMontage(IsMeleeMode() ? MeleeAttackMontage : SecondaryAttackMontage);
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ABsScythe::MeleeAttackFinished, AttackDuration, false);
}

void ABsScythe::SecondaryAttack()
{
	// Spawn projectile
	OnSecondaryAttack();
	
	if (GrappleHookComponent && bAttachedToGrapplePoint)
	{
		GrappleHookComponent->PullOwnerToLocation();
	}
}

void ABsScythe::OnMeleeAttack_Implementation()
{
	
}

void ABsScythe::OnRangedAttack_Implementation()
{
	
}

void ABsScythe::OnSecondaryAttack_Implementation()
{
	
}

void ABsScythe::SetWeaponMode(EScytheWeaponMode NewMode)
{
	WeaponMode = NewMode;
	OnWeaponModeChanged.Broadcast();
}

void ABsScythe::NextWeaponMode()
{
	// Disabled until more weapon modes have been added
	// return;
	
	// TODO: State, this should be a CanChangeWeaponMode() check
	if (!bThrown)
	{
		SetWeaponMode(WeaponMode == EScytheWeaponMode::ESWM_Range ? EScytheWeaponMode::ESWM_Shotgun : EScytheWeaponMode::ESWM_Range);
	}
	
}

void ABsScythe::SetAttacking(bool bNewAttacking)
{
	bIsAttacking = bNewAttacking;
}

void ABsScythe::StartGrappling()
{
	bGrappling = true;
}

void ABsScythe::StopGrappling()
{
	bGrappling = false;
}

void ABsScythe::Throw()
{
	Super::Throw();

	PlayMontage(ThrowMontage);
	
	if (bThrown)
	{
		bReturningToOwner = true;
		bAttachedToGrapplePoint = false;
		if (GrappleHookComponent)
		{
			GrappleHookComponent->DetachGrappleHook();
		}
		return;
	}

	ThrowDirection = GetActorForwardVector();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ThrowStartLocation = GetActorLocation();

	bThrown = true;
	bCanAttack = false;
}

void ABsScythe::Equip()
{
	Super::Equip();

	SetActorRelativeLocation(FVector::ZeroVector);

	// Can replace this with a default mode config eventually
	SetWeaponMode(EScytheWeaponMode::ESWM_Range);
}

void ABsScythe::DecapitatedActor(ABsSeveredHeadBase* DecapitatedHead)
{
	if (HasAttachedHead()) return;

	AttachSeveredHead(DecapitatedHead);
}

void ABsScythe::OnGrappleHookDetached()
{
	bAttachedToGrapplePoint = false;
	bReturningToOwner = true;
	PlayMontage(ThrowMontage);
}

void ABsScythe::OnScytheOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (bThrown)
	{
		if (OtherActor == GetOwner())
		{
			return;
		}
		
		if (UBsGrapplePointComponent* GrapplePointComponent = OtherActor->GetComponentByClass<UBsGrapplePointComponent>())
		{
			AttachToComponent(GrapplePointComponent, FAttachmentTransformRules::KeepWorldTransform);
			bAttachedToGrapplePoint = true;
			GrappleHookComponent->AttachToGrapplePoint(GrapplePointComponent);
			ClearMontage(ThrowMontage);
			OnWeaponThrowEnd.Broadcast();

			return;
		}

		if (ABsSeveredHeadBase* SeveredHead = Cast<ABsSeveredHeadBase>(OtherActor))
		{
			// If there isn't an attached head, attempt to attach this one.
			if (!AttachedSeveredHead)
			{
				AttachSeveredHead(SeveredHead);
				bReturningToOwner = true;
			}
			return;
		}

		OtherActor->TakeDamage(MeleeDamage, FDamageEvent(ThrownDamageType), GetInstigatorController(), this);
		bReturningToOwner = true;
	}
}

void ABsScythe::OnMeleeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (!bThrown && bIsAttacking)
	{
		// Only damage actors once per attack.
		if (!MeleeHitActors.Contains(OtherActor) && GetOwner() != OtherActor)
		{
			MeleeHitActors.Add(OtherActor);
			OtherActor->TakeDamage(MeleeDamage, FDamageEvent(UBsMeleeScytheDamageType::StaticClass()), GetInstigatorController(), this);
		}
	}
}

FTransform ABsScythe::GetProjectileSpawnTransform() const
{
	FTransform SpawnTransform = GetActorTransform();
	if (WeaponMesh)
	{
		SpawnTransform = WeaponMesh->GetSocketTransform(FName("MuzzleSocket"));
	}
	
	SpawnTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	// Calculate the rotation of the projectile
	UWorld* World = GetWorld();
	if (ProjectileAimComponent && World)
	{
		// Calculate the end location of the projectile and set the rotation to face it
		FHitResult HitResult;
		const FVector StartLocation = ProjectileAimComponent->GetComponentLocation();
		const FVector EndLocation = StartLocation + (ProjectileAimComponent->GetForwardVector() * 10000.0f);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.AddIgnoredActor(GetOwner());

		World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParams);
		const FVector TargetLocation = HitResult.bBlockingHit ? HitResult.Location : EndLocation;
		SpawnTransform.SetRotation((TargetLocation- SpawnTransform.GetLocation()).Rotation().Quaternion());
	}
	else
	{
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());
	}

	// Apply spread if needed
	if (WeaponMode == EScytheWeaponMode::ESWM_Shotgun)
	{
		FRotator SpreadRotation = SpawnTransform.GetRotation().Rotator();
		SpreadRotation.Yaw += FMath::RandRange(-ShotgunConfig.PelletSpread, ShotgunConfig.PelletSpread);
		SpreadRotation.Pitch += FMath::RandRange(-ShotgunConfig.PelletSpread, ShotgunConfig.PelletSpread);
		SpawnTransform.SetRotation(SpreadRotation.Quaternion());
	}

	return SpawnTransform;
}