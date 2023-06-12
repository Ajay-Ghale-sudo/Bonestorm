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

// Called when the game starts or when spawned
void ABsEventTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABsEventTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

