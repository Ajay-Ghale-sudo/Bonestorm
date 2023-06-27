// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner/BsEnemySpawner.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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
}

void ABsEnemySpawner::SpawnFXAtLocation()
{
	if (SpawnFX)
	{
		SpawnFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			SpawnFX,
			GetActorLocation(),
			GetActorRotation(),
			GetActorScale(),
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true);
	}
}

ABsEnemyBase* ABsEnemySpawner::SpawnEnemy()
{
	UWorld* World = GetWorld();
	ABsEnemyBase* SpawnedEnemy = nullptr;
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform SpawnTransform = GetTransform();
		SpawnedEnemy = Cast<ABsEnemyBase>(World->SpawnActor(SpawnEnemyClass, &SpawnTransform));
		if (SpawnedEnemy)
		{
			SpawnFXAtLocation();
		}
	}
	return SpawnedEnemy;
}