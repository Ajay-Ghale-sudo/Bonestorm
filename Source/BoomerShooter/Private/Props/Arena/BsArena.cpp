// Fill out your copyright notice in the Description page of Project Settings.
#include "Props/Arena/BsArena.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Props/Door/BsDoorBase.h"
// Sets default values
ABsArena::ABsArena()
{
	ArenaVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ArenaVolume"));
	SetRootComponent(ArenaVolume);
}

// Called when the game starts or when spawned
void ABsArena::BeginPlay()
{
	Super::BeginPlay();
	if (ArenaVolume)
	{
		ArenaVolume->OnComponentBeginOverlap.AddDynamic(this, &ABsArena::OnArenaVolumeBeginOverlap);
	}
}

void ABsArena::OnArenaVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		if (Character->IsPlayerControlled())
		{
			StartArena();
		}
	}
}

void ABsArena::StartArena()
{
	bArenaActive = true;
	SetDoorsLocked(bArenaActive);
	ArenaVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnArenaStarted.Broadcast();
}

void ABsArena::EndArena()
{
	bArenaActive = false;
	SetDoorsLocked(bArenaActive);
	OnArenaFinished.Broadcast();
}

void ABsArena::SetDoorsLocked(bool bLockDoors)
{
	if (UWorld* World = GetWorld())
	{
		for (auto Door : LockableDoors)
		{
			Door->CloseDoor();
			Door->SetLocked(bLockDoors);
		}
	}
}