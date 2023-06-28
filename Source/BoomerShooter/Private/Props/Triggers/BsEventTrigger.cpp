// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Triggers/BsEventTrigger.h"
#include "Components/BoxComponent.h"

// Sets default values
ABsEventTrigger::ABsEventTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
}

void ABsEventTrigger::BeginPlay()
{
	Super::BeginPlay();
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABsEventTrigger::OnTriggerComponentBeginOverlap);
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABsEventTrigger::OnTriggerComponentEndOverlap);
	}
}

void ABsEventTrigger::Triggered_Implementation()
{
	
}

void ABsEventTrigger::OnTriggerComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (TriggerClass && OtherActor && OtherActor->IsA(TriggerClass))
	{
		Triggered();
	}
}

void ABsEventTrigger::OnTriggerComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (TriggerClass && OtherActor && OtherActor->IsA(TriggerClass))
	{
		Triggered();
	}
}

