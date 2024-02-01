// Fill out your copyright notice in the Description page of Project Settings.

#include "Props/Triggers/BsBaseTrigger.h"
#include "Components/BoxComponent.h"

// Sets default values
ABsBaseTrigger::ABsBaseTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName("Trigger");
	SetRootComponent(TriggerBox);
}

// Called when the game starts or when spawned
void ABsBaseTrigger::BeginPlay()
{
	Super::BeginPlay();

	ResetTrigger();
}

void ABsBaseTrigger::ResetTrigger()
{
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABsBaseTrigger::OnTriggerComponentBeginOverlap);
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABsBaseTrigger::OnTriggerComponentEndOverlap);
	}
}

void ABsBaseTrigger::StartTrigger(AActor* TriggeredActor)
{
	OnTriggerStart.Broadcast(TriggeredActor);
}

void ABsBaseTrigger::EndTrigger(AActor* TriggeredActor)
{
	OnTriggerEnd.Broadcast(TriggeredActor);
}

void ABsBaseTrigger::OnTriggerComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (TriggerClass && OtherActor && OtherActor->IsA(TriggerClass))
	{
		StartTrigger(OtherActor);

		// Remove the trigger collision if it will be manually reset.
		if (bManualReset && TriggerBox)
		{
			TriggerBox->OnComponentBeginOverlap.RemoveDynamic(this, &ABsBaseTrigger::OnTriggerComponentBeginOverlap);
			TriggerBox->OnComponentEndOverlap.RemoveDynamic(this, &ABsBaseTrigger::OnTriggerComponentEndOverlap);
		}
	}
}

void ABsBaseTrigger::OnTriggerComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (TriggerClass && OtherActor && OtherActor->IsA(TriggerClass))
	{
		EndTrigger(OtherActor);
	}
}
