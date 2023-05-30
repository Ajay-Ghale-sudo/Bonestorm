// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/BsEnemySpawner.h"


// Sets default values
ABsEnemySpawner::ABsEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ABsEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void ABsEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	if (bSpawnOnBeginPlay) StartSpawnTimer();
}

void ABsEnemySpawner::StartSpawnTimer()
{
	UWorld* World = GetWorld();
	if (World && SpawnEnemyClass)
	{
        FTimerManager& TimerManager = GetWorldTimerManager();
        TimerManager.SetTimer(EnemySpawnTimerHandle, this, &ABsEnemySpawner::SpawnEnemy, SpawnInterval, true);
	}
}

void ABsEnemySpawner::SpawnEnemy()
{
	UWorld* World = GetWorld();
	if (World && CurrentSpawnAmount < MaxSpawnAmount)
	{
		FTransform SpawnTransform = GetTransform();
		World->SpawnActor(SpawnEnemyClass, &SpawnTransform);
		CurrentSpawnAmount++;
	}
	StopSpawnTimer();
}

void ABsEnemySpawner::StopSpawnTimer()
{
	EnemySpawnTimerHandle.Invalidate();
}