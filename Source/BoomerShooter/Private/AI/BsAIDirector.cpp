﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BsAIDirector.h"
#include "Kismet/GameplayStatics.h"
#include "Props/Arena/BsArena.h"
#include "Character/Enemy/BsEnemyBase.h"


void UBsAIDirector::Init()
{
	InitEnemies();
	InitArenas();
	
	if (const UWorld* World = GetWorld())
	{
		OnActorSpawnedDelegateHandle = World->AddOnActorSpawnedHandler(
			FOnActorSpawned::FDelegate::CreateUObject(this, &UBsAIDirector::OnActorSpawned));
	}	
}

void UBsAIDirector::InitArenas()
{
	if (const UWorld* World = GetWorld())
	{
		TArray<AActor*> ArenaActors;
		UGameplayStatics::GetAllActorsOfClass(World, ABsArena::StaticClass(), ArenaActors);

		for (AActor* ArenaActor : ArenaActors)
		{
			if (ABsArena* Arena = Cast<ABsArena>(ArenaActor))
			{
				Arena->OnThisArenaStarted.AddDynamic(this, &UBsAIDirector::OnArenaStarted);
				Arena->OnThisArenaFinished.AddDynamic(this, &UBsAIDirector::OnArenaFinished);
				Arenas.Add(Arena);
			}
		}
	}
}

void UBsAIDirector::InitEnemies()
{
	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABsEnemyBase::StaticClass(), EnemyActors);
	for (const auto Enemy : EnemyActors)
	{
		OnActorSpawned(Enemy);
	}
}

void UBsAIDirector::OnArenaStarted(ABsArena* StartedArena)
{
	if (ActiveArena)
	{
		ActiveArena->OnThisArenaStarted.RemoveDynamic(this, &UBsAIDirector::OnArenaStarted);
		ActiveArena->OnThisArenaFinished.RemoveDynamic(this, &UBsAIDirector::OnArenaFinished);
	}

	ActiveArena = StartedArena;
}

void UBsAIDirector::OnArenaFinished(ABsArena* StartedArena)
{
	if (ActiveArena == StartedArena)
	{
		ActiveArena->OnThisArenaStarted.RemoveDynamic(this, &UBsAIDirector::OnArenaStarted);
		ActiveArena->OnThisArenaFinished.RemoveDynamic(this, &UBsAIDirector::OnArenaFinished);
		ActiveArena = nullptr;
	}
}

void UBsAIDirector::OnActorSpawned(AActor* SpawnedActor)
{
	if (ABsEnemyBase* Enemy = Cast<ABsEnemyBase>(SpawnedActor))
	{
		Enemy->OnThisEnemyDeath.AddDynamic(this, &UBsAIDirector::OnEnemyDeath);
		ActiveEnemies.Add(Enemy);
	}
}

void UBsAIDirector::OnEnemyDeath(ABsEnemyBase* Enemy)
{
	if (!Enemy) return;
	
	Enemy->OnThisEnemyDeath.RemoveDynamic(this, &UBsAIDirector::OnEnemyDeath);
	ActiveEnemies.Remove(Enemy);
}


