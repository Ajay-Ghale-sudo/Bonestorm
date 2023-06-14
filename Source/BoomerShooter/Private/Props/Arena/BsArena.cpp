﻿// Fill out your copyright notice in the Description page of Project Settings.
#include "Props/Arena/BsArena.h"
#include "Character/Enemy/BsSkeleton.h"
#include "Component/BsHealthComponent.h"
#include "Components/BoxComponent.h"
#include "Data/BsArenaConfig.h"
#include "GameFramework/Character.h"
#include "Props/Door/BsDoorBase.h"
// Sets default values
ABsArena::ABsArena()
{
	PrimaryActorTick.bCanEverTick = false;
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
	NextWave();
}

void ABsArena::EndArena()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Ending Arena")));
	bArenaActive = false;
	SetDoorsLocked(bArenaActive);
	OnArenaFinished.Broadcast();
}

void ABsArena::ActivateSpawner()
{
	for (auto Spawner : ArenaConfig.EnemySpawnPoints)
	{
		// Make array of spawned enemies (for now, skeletons).
		if (ABsEnemyBase* Enemy = Spawner->SpawnEnemy())
		{
			ArenaConfig.SpawnedEnemies.Add(Enemy);
			if (UBsHealthComponent* HealthComponent = Enemy->FindComponentByClass<UBsHealthComponent>())
			{
				HealthComponent->OnDeath.AddDynamic(this, &ABsArena::OnSpawnedEnemyDeath);
			}
		}
		else
		{
			// @TODO Handle if enemy failed to spawn, currently covered in spawner by spawnparams, but should be done here.
		}
	}
}

void ABsArena::OnSpawnedEnemyDeath()
{
	for (int32 index = 0; index < ArenaConfig.SpawnedEnemies.Num(); ++index)
	{
		ABsEnemyBase* Enemy = ArenaConfig.SpawnedEnemies[index];
		if (!Enemy || !Enemy->IsValidLowLevel() ||!Enemy->GetIsAlive())
		{
			ArenaConfig.SpawnedEnemies.RemoveAt(index);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Amount of enemies remaining in wave: %d"), ArenaConfig.SpawnedEnemies.Num()));
	if (ArenaConfig.SpawnedEnemies.Num() > 0)
	{
		// If number of spawned enemies is greater than zero, do nothing.
	}
	else
	{
		NextWave();
	}
}

void ABsArena::NextWave()
{
	if (++ArenaConfig.CurrentArenaWave <= ArenaConfig.MaxArenaWaves)
	{
		if (ArenaConfig.SpawnDelay <= 0)
		{
			ActivateSpawner();
		}
		else
		{
			GetWorldTimerManager().SetTimer(ArenaConfig.NextWaveSpawnTimerHandle, this, &ABsArena::ActivateSpawner, ArenaConfig.SpawnDelay, false);
		}
	}
	else
	{
		EndArena();
	}
}

void ABsArena::SetDoorsLocked(bool bLockDoors)
{
	for (auto const Door : LockableDoors)
	{
		Door->CloseDoor();
		Door->SetLocked(bLockDoors);
	}
}