#include "Weapon/Scythe/BsScythe.h"

#include "Camera/CameraComponent.h"
#include "Component/BsGrappleHookComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/ReceiveDamage.h"
#include "Weapon/Projectile/BsGrappleProjectile.h"
#include "Weapon/Projectile/BsProjectileBase.h"

ABsScythe::ABsScythe()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MeleeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeCollision"));
	MeleeCollision->SetupAttachment(WeaponMesh, FName("BladeSocket"));
	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeleeCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

	GrappleHookComponent = CreateDefaultSubobject<UBsGrappleHookComponent>(TEXT("GrappleHookComponent"));
	
}

void ABsScythe::BeginPlay()
{
	Super::BeginPlay();
	
	if (MeleeCollision)
	{
		MeleeCollision->OnComponentBeginOverlap.AddDynamic(this, &ABsScythe::OnScytheOverlap);
	}

	if (GrappleHookComponent)
	{
		GrappleHookComponent->SetGrappleFXAttachPoint(WeaponMesh);
	}	
}

void ABsScythe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ThrowTick(DeltaTime);
}

void ABsScythe::ThrowTick(float DeltaTime)
{
	if (!bThrown || bAttachedToGrapplePoint) return;

	FVector Direction = GetActorForwardVector();
	if (bReturningToOwner && Owner)
	{
		if (FVector::Dist(GetActorLocation(), Owner->GetActorLocation()) < ReturnDistanceThreshold)
		{
			bThrown = false;
			bReturningToOwner = false;
			OnWeaponCaught.Broadcast();
			WeaponMesh->PlayAnimation(IdleAnimation, true);
			bCanAttack = true;
			return;			
		}
		Direction = Owner->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
	}
	
	SetActorLocation(GetActorLocation() + (Direction * DeltaTime * ThrowSpeed * (bReturningToOwner ? 2.f : 1.f)));
	const float Distance = FVector::Dist(GetActorLocation(), ThrowStartLocation);
	if (Distance >= ThrowDistance)
	{
		bReturningToOwner = true;
	}
}

void ABsScythe::Fire()
{
	if (!bCanAttack) return;
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
	if (!bCanAttack) return;
	Super::SecondaryFire();
	SecondaryAttack();
	UE_LOG(LogTemp, Log, TEXT("Firing secondary"))
}

void ABsScythe::RangeAttack()
{
	// Spawn Projectile
	OnRangedAttack();
	UWorld* World = GetWorld();
	if (ProjectileClass && World)
	{
		const FTransform SpawnTransform = GetProjectileSpawnTransform();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = GetInstigator();
		if (ABsProjectileBase* Projectile = Cast<ABsProjectileBase>(World->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams)))
		{
			Projectile->SetOwner(GetOwner());
		}
	}
}

void ABsScythe::MeleeAttack()
{
	OnMeleeAttack();
	if (WeaponMesh && MeleeAnimation)
	{
		WeaponMesh->PlayAnimation(MeleeAnimation, false);
		WeaponMesh->SetPlayRate(1.f);
	}
}

void ABsScythe::SecondaryAttack()
{
	// Spawn projectile
	OnSecondaryAttack();
	UWorld* World = GetWorld();	
	
	if (GrappleHookComponent && GetOwner())
	{
		GrappleHookComponent->DetachGrappleHook();
		// TODO: Not guaranteed to get the FPS camera. The grapple hook needs to know to spawn relative to the camera's aim.
		if (UCameraComponent* CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>())
		{
			// Float multiplies forward vector, designating start location of the grapple component, preventing self-collision
			FVector StartLocation = CameraComponent->GetComponentLocation() + (CameraComponent->GetForwardVector() * 100.f);
			GrappleHookComponent->FireGrappleHook(StartLocation, CameraComponent->GetForwardVector());
		}

		if (bAttachedToGrapplePoint)
		{
		}
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

	if (ThrowAnimation)
	{
		WeaponMesh->PlayAnimation(ThrowAnimation, true);
		WeaponMesh->SetPlayRate(3.f);
	}
	
	if (bThrown)
	{
		bReturningToOwner = true;
		bAttachedToGrapplePoint = false;
		return;
	}
	
	if (GrappleHookComponent)
	{
		GrappleHookComponent->DetachGrappleHook();
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

	if (WeaponMesh)
	{
		WeaponMesh->PlayAnimation(IdleAnimation, true);
	}

	SetActorRelativeLocation(FVector::ZeroVector);
}

void ABsScythe::OnScytheOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (bIsAttacking)
	{
 		if (IReceiveDamage* DamageActor = Cast<IReceiveDamage>(OtherActor))
		{
			DamageActor->ReceiveMeleeDamage(SweepResult, this, MeleeDamage);
		}
		else if (bReturningToOwner)
		{
			
		}
		else if (UBsGrapplePointComponent* GrapplePointComponent = OtherActor->GetComponentByClass<UBsGrapplePointComponent>())
		{
			if (!bThrown) return;

			AttachToComponent(GrapplePointComponent, FAttachmentTransformRules::KeepWorldTransform);
			bAttachedToGrapplePoint = true;
			WeaponMesh->Stop();
		}
	}
}

FTransform ABsScythe::GetProjectileSpawnTransform() const
{
	FTransform SpawnTransform = GetActorTransform();
	const FVector Direction = GetActorForwardVector();
	const FVector Location = SpawnTransform.GetLocation() + Direction * 300.0f;
	SpawnTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	SpawnTransform.SetLocation(Location);

	return SpawnTransform;
}
