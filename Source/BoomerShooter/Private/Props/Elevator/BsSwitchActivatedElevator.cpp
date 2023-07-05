// Fill out your copyright notice in the Description page of Project Settings.

#include "Props/Elevator/BsSwitchActivatedElevator.h"
#include "Component/BsSwitchTriggerComponent.h"

// Sets default values
ABsSwitchActivatedElevator::ABsSwitchActivatedElevator()
{
	SwitchTriggerComponent = CreateDefaultSubobject<UBsSwitchTriggerComponent>(TEXT("SwitchTrigger"));	
}

// Called when the game starts or when spawned
void ABsSwitchActivatedElevator::BeginPlay()
{
	Super::BeginPlay();
	BindSwitchTrigger();
}

void ABsSwitchActivatedElevator::BindSwitchTrigger()
{
	if (SwitchTriggerComponent)
	{
		SwitchTriggerComponent->OnSwitchTriggered.AddDynamic(this, &ABsSwitchActivatedElevator::OnSwitchTriggered);
	}
}

void ABsSwitchActivatedElevator::OnSwitchTriggered()
{
	SetActivated(true);
}