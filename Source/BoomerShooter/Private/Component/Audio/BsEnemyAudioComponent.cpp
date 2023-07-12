// Fill out your copyright notice in the Description page of Project Settings.
#include "Component/Audio/BsEnemyAudioComponent.h"
#include "Character/Enemy/BsEnemyBase.h"
#include "Component/BsHealthComponent.h"

// Sets default values for this component's properties
UBsEnemyAudioComponent::UBsEnemyAudioComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UBsEnemyAudioComponent::BeginPlay()
{
	EnemyOwner = Cast<ABsEnemyBase>(GetOwner());
	Super::BeginPlay();
}

void UBsEnemyAudioComponent::BindEvents()
{
	if (!EnemyOwner) return;
	
	EnemyOwner->OnHitStun.AddUObject(this, &UBsEnemyAudioComponent::OnHitStun);
	EnemyOwner->OnDeath.AddUObject(this, &UBsEnemyAudioComponent::OnDeath);
	
	if (UBsHealthComponent* HealthComponent = EnemyOwner->GetHealthComponent())
	{
		HealthComponent->OnTookDamageEvent.AddDynamic(this, &UBsEnemyAudioComponent::OnTookDamage);
	}
}

void UBsEnemyAudioComponent::UnbindEvents()
{
	if (!EnemyOwner) return;
	
	EnemyOwner->OnHitStun.RemoveAll(this);
	EnemyOwner->OnDeath.RemoveAll(this);

	if (UBsHealthComponent* HealthComponent = EnemyOwner->GetHealthComponent())
	{
		HealthComponent->OnTookDamageEvent.RemoveAll(this);
	}
}

void UBsEnemyAudioComponent::OnHitStun()
{
	PlaySound(EnemyAudioData.HitStunSound);		
}

void UBsEnemyAudioComponent::OnDeath()
{
	PlaySound(EnemyAudioData.DeathSound);
}

void UBsEnemyAudioComponent::OnTookDamage()
{
	PlaySound(EnemyAudioData.TookDamageSound);
}