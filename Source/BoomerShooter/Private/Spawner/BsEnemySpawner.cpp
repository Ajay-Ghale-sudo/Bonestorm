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
	SpawnTimer();
}

void ABsEnemySpawner::SpawnTimer()
{
	UWorld* World = GetWorld();
	if (World && SpawnedEnemyClass)
	{
		FTransform SpawnTransform = GetTransform();
        FTimerManager& TimerManager = GetWorldTimerManager();
		/*
		 * Would not repeat the timer without a delegate, will be refactored to take spawn conditions from AI director later
		*/
        TimerManager.SetTimer(EnemySpawnTimer, this, &ABsEnemySpawner::SpawnEnemy, SpawnInterval, true);
        World->SpawnActor(SpawnedEnemyClass, &SpawnTransform);
	}
}

void ABsEnemySpawner::SpawnEnemy()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FTransform SpawnTransform = GetTransform();
		World->SpawnActor(SpawnedEnemyClass, &SpawnTransform);
	}
}