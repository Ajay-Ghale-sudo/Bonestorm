// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Triggers/BsSecretTrigger.h"
#include "Character/BsCharacter.h"


// Sets default values
ABsSecretTrigger::ABsSecretTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerClass = ABsCharacter::StaticClass();
	bManualReset = true;
}

void ABsSecretTrigger::StartTrigger(AActor* TriggeredActor)
{
	Super::StartTrigger(TriggeredActor);

	if (const ABsCharacter* Character = Cast<ABsCharacter>(TriggeredActor))
	{
		Character->TriggerSecret();
	}
}
