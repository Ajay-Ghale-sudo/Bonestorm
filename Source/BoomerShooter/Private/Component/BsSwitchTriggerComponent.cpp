// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BsSwitchTriggerComponent.h"
#include "Component/BsToggleSwitchComponent.h"


// Sets default values for this component's properties
UBsSwitchTriggerComponent::UBsSwitchTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UBsSwitchTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const auto Actor : SwitchActors)
	{
		if (Actor)
		{
			const auto Switch = Actor->FindComponentByClass<UBsToggleSwitchComponent>();
			if (Switch)
			{
				Switches.Add(Switch);
				Switch->OnToggle.AddDynamic(this, &UBsSwitchTriggerComponent::OnSwitchToggled);
			}
		}
	}
	// ...
	
}

void UBsSwitchTriggerComponent::CheckSwitches()
{
	if (bTriggered && bTriggerOnce) return;

	bool bAllSwitchesOn = true;
	for (const auto Switch : Switches)
	{
		if (!Switch->IsOn())
		{
			bAllSwitchesOn = false;
			break;
		}
	}
	bTriggered = bAllSwitchesOn;
	if (bTriggered)
	{
		Triggered();
	}
}


void UBsSwitchTriggerComponent::OnSwitchToggled()
{
	CheckSwitches();
}

void UBsSwitchTriggerComponent::TriggeredInternal()
{
	OnSwitchTriggered.Broadcast();
}

void UBsSwitchTriggerComponent::Triggered_Implementation()
{
	TriggeredInternal();
}