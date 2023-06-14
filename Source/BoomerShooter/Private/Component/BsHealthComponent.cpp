﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BsHealthComponent.h"


// Sets default values for this component's properties
UBsHealthComponent::UBsHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBsHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (AActor *Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UBsHealthComponent::ProcessDamage);
	}
}

void UBsHealthComponent::ProcessDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{

	// TODO: Damage Type should determine if self damage is allowed
	if (!DamageCauser || DamageCauser == DamagedActor || DamagedActor == DamageCauser->GetOwner())
	{
		return;
	}

	if (DamageTypesToIgnore.Contains(DamageType->GetClass()))
	{
		return;
	}
	
	OnTookDamage.Broadcast();
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	if (CurrentHealth <= 0.f)
	{
		OnDeath.Broadcast();
	}
	OnHealthChanged.Broadcast();
}


// Called every frame
void UBsHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

