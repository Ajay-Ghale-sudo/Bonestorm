#include "Weapon/Scythe/BsScythe.h"

#include "BoomerShooter.h"
#include "Camera/CameraComponent.h"
#include "Component/BsGrappleHookComponent.h"
#include "Components/BoxComponent.h"
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
			ClearMontage();
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
		return RangedConfig.bCanFire;
	}
	
	return true;
}

void ABsScythe::EnableRangedFire()
{
	RangedConfig.bCanFire = true;
}

void ABsScythe::Fire()
{
	if (!CanAttack()) return;
	Super::Fire();
	
	if (WeaponMode == EScytheWeaponMode::ESWM_Melee)
	{
		MeleeAttack();
	}
	else if (WeaponMode == EScytheWeaponMode::ESWM_Range)
	{
		RangeAttack();
	}
}

void ABsScythe::SecondaryFire()
{
	SecondaryAttack();
}

void ABsScythe::RangeAttack()
{
	// Spawn Projectile
	UWorld* World = GetWorld();
	if (RangedConfig.ProjectileClass && World && RangedConfig.bCanFire)
	{
		const FTransform SpawnTransform = GetProjectileSpawnTransform();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = GetInstigator();
		ABsProjectileBase* Projectile = nullptr;
		
		if (AttachedSeveredHead)
		{
			// TODO: Attached Severed Head mechanics
			// Add effect to Projectile?
			// Change the projectile that is spawned?
			Projectile = AttachedSeveredHead->CreateProjectile(RangedConfig.ProjectileClass, SpawnTransform, SpawnParams);
		}
		if (!Projectile)
		{
			Projectile = Cast<ABsProjectileBase>(World->SpawnActor(RangedConfig.ProjectileClass, &SpawnTransform, SpawnParams));
		}
		
		if (Projectile)
		{
			Projectile->SetOwner(GetOwner());
			Projectile->SetInstigator(GetInstigator());
			PlayMontage(RangedAttackMontage);
			OnRangedAttack();
		}

		RangedConfig.bCanFire = false;
		World->GetTimerManager()
			.SetTimer(
				RangedConfig.FireRateTimerHandle,
				this,
				&ABsScythe::EnableRangedFire,
				RangedConfig.FireRate,
				false
		);
	}
}

void ABsScythe::MeleeAttack()
{
	OnMeleeAttack();
	PlayMontage(MeleeAttackMontage);
	bCanAttack = false;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ABsScythe::EnableAttack, AttackDuration, false);
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
}

void ABsScythe::NextWeaponMode()
{
	WeaponMode = WeaponMode == EScytheWeaponMode::ESWM_Melee ? EScytheWeaponMode::ESWM_Range : EScytheWeaponMode::ESWM_Melee;
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

	bIsAttacking = true;
	bThrown = true;
	bCanAttack = false;
}

void ABsScythe::Equip()
{
	Super::Equip();

	SetActorRelativeLocation(FVector::ZeroVector);
}

void ABsScythe::OnGrappleHookDetached()
{
	bAttachedToGrapplePoint = false;
	bReturningToOwner = true;
}

void ABsScythe::OnScytheOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (bThrown)
	{
		
 		if (IReceiveDamage* DamageActor = Cast<IReceiveDamage>(OtherActor))
		{
 			if (!bIsAttacking) return;
 			
			DamageActor->ReceiveMeleeDamage(SweepResult, this, MeleeDamage);
 			return;
		}

		if (OtherActor == GetOwner())
		{
			return;
		}
		
		if (UBsGrapplePointComponent* GrapplePointComponent = OtherActor->GetComponentByClass<UBsGrapplePointComponent>())
		{
			if (!bThrown) return;

			AttachToComponent(GrapplePointComponent, FAttachmentTransformRules::KeepWorldTransform);
			bAttachedToGrapplePoint = true;
			GrappleHookComponent->AttachToGrapplePoint(GrapplePointComponent);
			ClearMontage();

			return;
		}

		if (ABsSeveredHeadBase* SeveredHead = Cast<ABsSeveredHeadBase>(OtherActor))
		{
			// If there isn't an attached head, attempt to attach this one.
			if (!AttachedSeveredHead)
			{
				AttachSeveredHead(SeveredHead);
			}
			return;
		}

		bReturningToOwner = true;
	}
}

void ABsScythe::OnMeleeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (bIsAttacking)
	{
		if (IReceiveDamage* DamageActor = Cast<IReceiveDamage>(OtherActor))
		{
			DamageActor->ReceiveMeleeDamage(SweepResult, this, MeleeDamage);
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
	SpawnTransform.SetRotation(GetActorRotation().Quaternion());

	return SpawnTransform;
}