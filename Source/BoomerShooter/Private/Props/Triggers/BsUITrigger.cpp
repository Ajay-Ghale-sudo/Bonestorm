// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Triggers/BsUITrigger.h"

#include "Character/BsCharacter.h"
#include "Components/BoxComponent.h"
#include "Subsystem/UI/BsUISubsystem.h"


// Sets default values
ABsUITrigger::ABsUITrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName("Trigger");
	SetRootComponent(TriggerBox);
}

// Called when the game starts or when spawned
void ABsUITrigger::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABsUITrigger::OnTriggerComponentBeginOverlap);
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABsUITrigger::OnTriggerComponentEndOverlap);
	}
	
}

void ABsUITrigger::OnTriggerComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	// TODO: This could be handled by collision channel. Only the player should be able to collide with this trigger.
	if (OtherActor->IsA(ABsCharacter::StaticClass()))
	{
		if (const UBsUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UBsUISubsystem>())
		{
			UISubsystem->OnUIEventTriggered.Broadcast(UIGameplayTag);
		}
	}
}

void ABsUITrigger::OnTriggerComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bEndEventOnOverlapEnd) return;

	// TODO: This could be handled by collision channel. Only the player should be able to collide with this trigger.
	if (OtherActor->IsA(ABsCharacter::StaticClass()))
	{
		if (const UBsUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UBsUISubsystem>())
		{
			UISubsystem->OnUIEventEnded.Broadcast(UIGameplayTag);
		}
	}
}