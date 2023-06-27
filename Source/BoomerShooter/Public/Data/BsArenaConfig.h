#pragma once
#include "Spawner/BsEnemySpawner.h"
#include "BsArenaConfig.generated.h"

USTRUCT(BlueprintType)
struct FBsArenaConfig
{
	GENERATED_BODY()

	/**
	 * @brief Array of enemies to spawn determined in-editor by the Spawner
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena Config", meta = (AllowPrivateAccess = "true"))
	TArray<ABsEnemySpawner*> EnemySpawnPoints;

	/**
	 * @brief Array of currently spawned enemies
	 **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arena Config", meta = (AllowPrivateAccess = "true"))
	TArray<ABsEnemyBase*> SpawnedEnemies;
	
	/**
	 * @brief On beginning a new wave, this delay determines how long it is before they spawn. Set in editor.
	 *		 (stops enemies from spawning into player damage and dying instantly)
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena Config", meta = (AllowPrivateAccess = "true"))
	float RoundDelay = 0.f;

	/*
	 * @brief This float determines individual spawn delay for individual spawners, allowing more configurable spawns.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena Config", meta = (AllowPrivateAccess = "true"))
	float SpawnDelay = 0.f;
		
	/**
	 * @brief Maximum waves of the arena
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena Config", meta = (AllowPrivateAccess = "true"))
	int32 MaxArenaWaves;

	/**
	 * @brief Current wave number in the arena
	 **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arena Config", meta = (AllowPrivateAccess = "true"))
	int32 CurrentArenaWave = 0;

	/**
	 * @brief Timer handle for spawn delay
	 **/
	FTimerHandle NextWaveSpawnTimerHandle;	
};