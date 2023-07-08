// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Audio/BsCharacterAudioComponent.h"
#include "Character/BsCharacter.h"
#include "Components/AudioComponent.h"

void UBsCharacterAudioComponent::BeginPlay()
{
	CharacterOwner = Cast<ABsCharacter>(GetOwner());
	Super::BeginPlay();
}

void UBsCharacterAudioComponent::BindEvents()
{
	if (!CharacterOwner) return;

	CharacterOwner->OnDash.AddUObject(this, &UBsCharacterAudioComponent::OnDash);
	CharacterOwner->OnSlideStart.AddUObject(this, &UBsCharacterAudioComponent::OnSlideStart);
	CharacterOwner->OnSlideStop.AddUObject(this, &UBsCharacterAudioComponent::OnSlideStop);
	CharacterOwner->OnArenaStarted.AddUObject(this, &UBsCharacterAudioComponent::PlayCombatMusic);
	CharacterOwner->OnArenaEnded.AddUObject(this, &UBsCharacterAudioComponent::StopCombatMusic);
}

void UBsCharacterAudioComponent::UnbindEvents()
{
	if (!CharacterOwner) return;

	CharacterOwner->OnDash.RemoveAll(this);
}

void UBsCharacterAudioComponent::OnDash()
{
	PlaySound(CharacterAudioData.DashSound);
}

void UBsCharacterAudioComponent::OnSlideStart()
{
	CharacterAudioData.SlideAudioComponent = PlaySound(CharacterAudioData.SlideSound);
}

void UBsCharacterAudioComponent::OnSlideStop()
{
	if (CharacterAudioData.SlideAudioComponent)
	{
		// Stop the sound on the next tick
		CharacterAudioData.SlideAudioComponent->StopDelayed(.1f);
	}

	CharacterAudioData.SlideAudioComponent = nullptr;
}

void UBsCharacterAudioComponent::PlayCombatMusic()
{
	if (CharacterAudioData.CombatMusicAudioComponent)
	{
		CharacterAudioData.CombatMusicAudioComponent->Play();
	}
	else
	{
		CharacterAudioData.CombatMusicAudioComponent = PlaySound(CharacterAudioData.CombatMusic, nullptr, nullptr);
	}

	if (CharacterAudioData.CombatMusicAudioComponent)
	{
		if (!CharacterAudioData.CombatMusicAudioComponent->OnAudioFinished.IsAlreadyBound(this, &UBsCharacterAudioComponent::PlayCombatMusic))
		{
			CharacterAudioData.CombatMusicAudioComponent->OnAudioFinished.AddDynamic(this, &UBsCharacterAudioComponent::PlayCombatMusic);
		}
	}
}

void UBsCharacterAudioComponent::StopCombatMusic()
{
	if (CharacterAudioData.CombatMusicAudioComponent)
	{
		CharacterAudioData.CombatMusicAudioComponent->OnAudioFinished.RemoveAll(this);
		CharacterAudioData.CombatMusicAudioComponent->Stop();
		CharacterAudioData.CombatMusicAudioComponent->DestroyComponent();
	}
}
