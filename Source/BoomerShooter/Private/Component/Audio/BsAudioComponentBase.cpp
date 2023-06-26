// Fill out your copyright notice in the Description page of Project Settings.
#include "Component/Audio/BsAudioComponentBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBsAudioComponentBase::UBsAudioComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

UAudioComponent* UBsAudioComponentBase::PlaySound(USoundBase* Sound)
{
	return PlaySound(Sound, AudioData.AttenuationSettings, AudioData.ConcurrencySettings);
}

UAudioComponent* UBsAudioComponentBase::PlaySound(USoundBase* Sound, USoundAttenuation* AttenuationSettings,
	USoundConcurrency* ConcurrencySettings)
{
	if (!Sound || !GetOwner()) return nullptr;

	return UGameplayStatics::SpawnSoundAttached(
		Sound,
		GetOwner()->GetRootComponent(),
		NAME_None,
		FVector::ZeroVector,
		EAttachLocation::SnapToTarget,
		true,
		1.f,
		1.f,
		0.f,
		AttenuationSettings,
		ConcurrencySettings,
		true
	);
}

// Called when the game starts
void UBsAudioComponentBase::BeginPlay()
{
	Super::BeginPlay();
	BindEvents();
}

void UBsAudioComponentBase::BindEvents()
{
	// Nothing by default
}

void UBsAudioComponentBase::UnbindEvents()
{
	// Nothing by default
}