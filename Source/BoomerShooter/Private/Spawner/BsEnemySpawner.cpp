// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/BsEnemySpawner.h"


// Sets default values
ABsEnemySpawner::ABsEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABsEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABsEnemySpawner::Spawn()
{
	UWorld* World = GetWorld();
	FTransform SpawnTransform = GetTransform();
	if (SpawnedEnemy && World)
	{
		World->SpawnActor(SpawnedEnemy, &SpawnTransform, SpawnParams);
	}
}

// Called every frame
void ABsEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

