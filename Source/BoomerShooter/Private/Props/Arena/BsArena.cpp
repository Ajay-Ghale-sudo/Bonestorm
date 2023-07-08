// Fill out your copyright notice in the Description page of Project Settings.
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
	OnThisArenaStarted.Broadcast(this);
	NextWave();
}

void ABsArena::EndArena()
{
	bArenaActive = false;
	SetDoorsLocked(bArenaActive);
	OnArenaFinished.Broadcast();
	OnThisArenaFinished.Broadcast(this);
}

void ABsArena::ActivateSpawner()
{
	for (const auto Spawner : ArenaConfig.EnemySpawnPoints)
	{
		// Make array of spawned enemies
		if (ABsEnemyBase* Enemy = Spawner->SpawnEnemy())
		{
			ArenaConfig.SpawnedEnemies.Add(Enemy);
			OnEnemySpawned.Broadcast(Enemy);
			if (UBsHealthComponent* HealthComponent = Enemy->FindComponentByClass<UBsHealthComponent>())
			{
				HealthComponent->OnDeath.AddDynamic(this, &ABsArena::OnSpawnedEnemyDeath);
			}
		}
		else
		{
			// @TODO Handle if enemy failed to spawn
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
		if (ArenaConfig.WaveDelay <= 0)
		{
			// TODO: Should check to see what round it is, if the spawner is active on this round, etc.
			ActivateSpawner();
		}
		else
		{
			GetWorldTimerManager().SetTimer(ArenaConfig.NextWaveSpawnTimerHandle, this, &ABsArena::ActivateSpawner, ArenaConfig.WaveDelay, false);
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