#include "Component/Audio/BsWeaponAudioComponent.h"
#include "Components/AudioComponent.h"
#include "Weapon/BsWeaponBase.h"

// Sets default values for this component's properties
UBsWeaponAudioComponent::UBsWeaponAudioComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UBsWeaponAudioComponent::BeginPlay()
{
	WeaponOwner = Cast<ABsWeaponBase>(GetOwner());
	Super::BeginPlay();
}

void UBsWeaponAudioComponent::BindEvents()
{
	if (!WeaponOwner) return;

	WeaponOwner->OnWeaponParry.AddUObject(this, &UBsWeaponAudioComponent::OnWeaponParry);
	WeaponOwner->OnWeaponCaught.AddUObject(this, &UBsWeaponAudioComponent::OnWeaponCaught);
	WeaponOwner->OnWeaponBlock.AddUObject(this, &UBsWeaponAudioComponent::OnWeaponBlock);
	WeaponOwner->OnWeaponThrow.AddUObject(this, &UBsWeaponAudioComponent::OnWeaponThrow);
	WeaponOwner->OnWeaponThrowEnd.AddUObject(this, &UBsWeaponAudioComponent::OnWeaponThrowEnd);
	WeaponOwner->OnWeaponEquip.AddUObject(this, &UBsWeaponAudioComponent::OnWeaponEquip);
	WeaponOwner->OnWeaponModeChanged.AddUObject(this, &UBsWeaponAudioComponent::OnWeaponModeChanged);
	WeaponOwner->OnWeaponFire.AddUObject(this, &UBsWeaponAudioComponent::OnWeaponFire);
	WeaponOwner->OnWeaponHit.AddUObject(this, &UBsWeaponAudioComponent::OnWeaponHit);
	WeaponOwner->OnWeaponHeal.AddUObject(this, &UBsWeaponAudioComponent::OnWeaponHeal);
	WeaponOwner->OnWeaponMeleeAttack.AddUObject(this, &UBsWeaponAudioComponent::OnMeleeAttack);
	WeaponOwner->OnWeaponHeadConsumed.AddUObject(this, &UBsWeaponAudioComponent::OnWeaponHeadConsumed);
}

void UBsWeaponAudioComponent::UnbindEvents()
{
	if (!WeaponOwner) return;

	WeaponOwner->OnWeaponParry.RemoveAll(this);
	WeaponOwner->OnWeaponCaught.RemoveAll(this);
	WeaponOwner->OnWeaponBlock.RemoveAll(this);
	WeaponOwner->OnWeaponThrow.RemoveAll(this);
	WeaponOwner->OnWeaponEquip.RemoveAll(this);
	WeaponOwner->OnWeaponModeChanged.RemoveAll(this);
	WeaponOwner->OnWeaponFire.RemoveAll(this);
	WeaponOwner->OnWeaponHit.RemoveAll(this);
	WeaponOwner->OnWeaponHeal.RemoveAll(this);
	WeaponOwner->OnWeaponMeleeAttack.RemoveAll(this);
	WeaponOwner->OnWeaponHeadConsumed.RemoveAll(this);
}

void UBsWeaponAudioComponent::OnWeaponCaught()
{
	OnWeaponThrowEnd();
	PlaySound(WeaponAudioData.WeaponCaughtSound);
}

void UBsWeaponAudioComponent::OnWeaponParry()
{
	PlaySound(WeaponAudioData.WeaponParrySound);
}

void UBsWeaponAudioComponent::OnWeaponBlock()
{
	PlaySound(WeaponAudioData.WeaponBlockSound);
}

void UBsWeaponAudioComponent::OnWeaponThrow()
{
	if (!WeaponAudioData.WeaponThrowAudioComponent)
	{
		WeaponAudioData.WeaponThrowAudioComponent = PlaySound(WeaponAudioData.WeaponThrowSound);
	}

	if (WeaponAudioData.WeaponThrowAudioComponent && !WeaponAudioData.WeaponThrowAudioComponent->IsPlaying())
	{
		WeaponAudioData.WeaponThrowAudioComponent->Play();
	}
}

void UBsWeaponAudioComponent::OnWeaponThrowEnd()
{
	if (WeaponAudioData.WeaponThrowAudioComponent)
	{
		WeaponAudioData.WeaponThrowAudioComponent->Stop();
		WeaponAudioData.WeaponThrowAudioComponent->DestroyComponent();
		WeaponAudioData.WeaponThrowAudioComponent = nullptr;
	}
}

void UBsWeaponAudioComponent::OnWeaponEquip()
{
	PlaySound(WeaponAudioData.WeaponEquipSound);
}

void UBsWeaponAudioComponent::OnWeaponModeChanged()
{
	PlaySound(WeaponAudioData.WeaponModeChangedSound);
}

void UBsWeaponAudioComponent::OnWeaponFire()
{
	PlaySound(WeaponAudioData.WeaponFireSound);
}

void UBsWeaponAudioComponent::OnWeaponHit()
{
	PlaySound(WeaponAudioData.WeaponHitSound);
}

void UBsWeaponAudioComponent::OnWeaponHeal()
{
	PlaySound(WeaponAudioData.WeaponHealSound);
}

void UBsWeaponAudioComponent::OnMeleeAttack()
{
	PlaySound(WeaponAudioData.WeaponMeleeAttackSound);
}

void UBsWeaponAudioComponent::OnWeaponHeadConsumed()
{
	PlaySound(WeaponAudioData.WeaponHeadConsumedSound);
}