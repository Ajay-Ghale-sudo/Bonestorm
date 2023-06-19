// Fill out your copyright notice in the Description page of Project Settings.
#include "Weapon/BsWeaponBase.h"
#include "Props/Head/BsSeveredHeadBase.h"
// Sets default values
ABsWeaponBase::ABsWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void ABsWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABsWeaponBase::EnableAttack()
{
	bCanAttack = true;
}

void ABsWeaponBase::PlayMontage(UAnimMontage* MontageToPlay) const
{
	if (!MontageToPlay || !WeaponMesh)
	{
		return;
	}

	if (UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance())
	{
		AnimInstance->Montage_Play(MontageToPlay);
	}
}

void ABsWeaponBase::ClearMontage(const UAnimMontage* MontageToClear /*= nullptr*/) const
{
	if (!WeaponMesh) return;
	if (UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance())
	{
		AnimInstance->Montage_Stop(0.1f, MontageToClear);
	}
}

bool ABsWeaponBase::CanAttack() const
{
	return bCanAttack;
}

void ABsWeaponBase::ConsumeHead()
{
	// Getting/setting player head charge current stopgap for preventing players from re-grabbing detached head and healing from it
	// @TODO Should probably be an animation that eats the head then destroys it, preventing this issue.
	if (AttachedSeveredHead && AttachedSeveredHead->GetCurrentCharge() > 0)
	{
		OnHeal.Broadcast(AttachedSeveredHead->GetHeadHealAmount());
		AttachedSeveredHead->Consume();
		DetachSeveredHead();
	}
}

void ABsWeaponBase::Fire()
{
	if (!bCanAttack)
	{
		return;
	}
	
	bCanAttack = false;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ABsWeaponBase::EnableAttack, AttackRate, false);
}

void ABsWeaponBase::SecondaryFire()
{
	if (!bCanAttack)
	{
		return;
	}

	bCanAttack = false;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ABsWeaponBase::EnableAttack, AttackRate, false);
}

float ABsWeaponBase::BlockIncomingDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	return Damage;
}

void ABsWeaponBase::StartBlock()
{
	
}

void ABsWeaponBase::StopBlock()
{
	
}

void ABsWeaponBase::NextWeaponMode()
{
		
}

void ABsWeaponBase::Drop()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (WeaponMesh)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
		WeaponMesh->SetPhysicsBlendWeight(0.f);
		WeaponMesh->SetSimulatePhysics(true);
	}
}

void ABsWeaponBase::Throw()
{
}

void ABsWeaponBase::Equip()
{

}

void ABsWeaponBase::AttachSeveredHead(ABsSeveredHeadBase* SeveredHead)
{
	if (!SeveredHead) return;
	
	if (AttachedSeveredHead)
	{
		DetachSeveredHead();
	}
	
	if (SeveredHead)
	{
		AttachedSeveredHead = SeveredHead;
		if (AttachedSeveredHead->bAttachable)
		{
			AttachedSeveredHead->GetHeadMesh()->SetSimulatePhysics(false);
			AttachedSeveredHead->SetAttached(true);
			AttachedSeveredHead->SetOwner(this);
			AttachedSeveredHead->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "SeveredHeadSocket");
			AttachedSeveredHead->SetActorRelativeScale3D(SeveredHeadScale);
			AttachedSeveredHead->OnDetachedHead.AddUObject(this, &ABsWeaponBase::DetachSeveredHead);
		}
	}
}

void ABsWeaponBase::DetachSeveredHead()
{
	if (AttachedSeveredHead)
	{
		AttachedSeveredHead->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AttachedSeveredHead->SetAttached(false);
		AttachedSeveredHead->SetOwner(nullptr);
		AttachedSeveredHead->GetHeadMesh()->SetSimulatePhysics(true);
		AttachedSeveredHead->GetHeadMesh()->ComponentVelocity = FVector(0.f, 0.f, 0.f);
		AttachedSeveredHead->GetHeadMesh()->AddImpulse(FVector(0.f, 0.f, 500.f), NAME_None, false);
		AttachedSeveredHead->OnDetachedHead.RemoveAll(this);
		AttachedSeveredHead->SetAttachable();
		AttachedSeveredHead = nullptr;
	}
}