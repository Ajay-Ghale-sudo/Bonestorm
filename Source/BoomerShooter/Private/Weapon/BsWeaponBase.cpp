// Fill out your copyright notice in the Description page of Project Settings.
#include "Weapon/BsWeaponBase.h"

#include "Component/Audio/BsAudioComponentBase.h"
#include "Component/Audio/BsWeaponAudioComponent.h"
#include "Props/Head/BsSeveredHeadBase.h"
// Sets default values
ABsWeaponBase::ABsWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(WeaponMesh);

	AudioComponent = CreateDefaultSubobject<UBsWeaponAudioComponent>("AudioComponent");
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

void ABsWeaponBase::ConsumeSeveredHead()
{
	if (AttachedSeveredHead)
	{
		const float Amount = AttachedSeveredHead->Consume();
		OnHeal.Broadcast(Amount);
		OnWeaponHeal.Broadcast();
		OnWeaponHeadConsumed.Broadcast();
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
	OnWeaponThrow.Broadcast();
}

void ABsWeaponBase::Equip()
{
	OnWeaponEquip.Broadcast();
	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}

void ABsWeaponBase::AttachSeveredHead(ABsSeveredHeadBase* SeveredHead)
{
	if (!SeveredHead || !SeveredHead->CanBeAttached()) return;
	
	if (AttachedSeveredHead)
	{
		DetachSeveredHead();
	}
	
	if (SeveredHead)
	{
		AttachedSeveredHead = SeveredHead;
		AttachedSeveredHead->AttachHeadToComponent(WeaponMesh, FName("SeveredHeadSocket"));
		AttachedSeveredHead->OnDetachedHead.AddUObject(this, &ABsWeaponBase::DetachSeveredHead);
		OnWeaponThrow.AddUObject(AttachedSeveredHead, &ABsSeveredHeadBase::HideWidget);
		OnWeaponCaught.AddUObject(AttachedSeveredHead, &ABsSeveredHeadBase::ShowWidget);
		OnWeaponHeadAttached.Broadcast(AttachedSeveredHead);
	}
}

void ABsWeaponBase::DetachSeveredHead()
{
	if (AttachedSeveredHead)
	{
		AttachedSeveredHead->OnDetachedHead.RemoveAll(this);
		OnWeaponThrow.RemoveAll(AttachedSeveredHead);
		OnWeaponCaught.RemoveAll(AttachedSeveredHead);
		AttachedSeveredHead->DetachHead();
		OnWeaponHeadDetached.Broadcast(AttachedSeveredHead);
		AttachedSeveredHead = nullptr;
	}
}