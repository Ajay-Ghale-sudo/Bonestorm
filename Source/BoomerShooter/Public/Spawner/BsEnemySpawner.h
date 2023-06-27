// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Enemy/BsEnemyBase.h"
#include "GameFramework/Actor.h"
#include "BsEnemySpawner.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class BOOMERSHOOTER_API ABsEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsEnemySpawner();
	virtual void Tick(float DeltaTime) override;
	
	/**
	 * @brief Timer for enemy spawns
	 **/
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void StartSpawnTimer();
	
	/**
	 * @brief Virtual function for spawning enemies
	 **/
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	virtual ABsEnemyBase* SpawnEnemy();

	UFUNCTION()
	void SpawnEnemyCallback();
	
	/**
	 * @brief Invalidates spawn timer, stopping enemies from spawning
	 **/
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void StopSpawnTimer();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	 * @brief Spawn VFX at location.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnFXAtLocation();
	
protected:
	/**
	 * @brief Enemy to spawn from this spawner (determined in blueprint)
	 **/
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<ABsEnemyBase> SpawnEnemyClass;

	/*
	 * @brief Niagara system for enemy spawn FX.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	UNiagaraSystem* SpawnFX;
	
	/*
	 * @brief Niagara component for enemy spawn SFX.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	UNiagaraComponent* SpawnFXComponent;

	/**
	 * @brief Interval in seconds that the timer spawns enemies
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	float SpawnInterval = 1.f;

	/**
	 * @brief Current amount of enemies spawned from this spawner
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int CurrentSpawnAmount = 0;

	/**
	 * @brief Maximum amount of enemies that can spawn from this spawner
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int MaxSpawnAmount = 5;
	
	/**
	 * @brief Should start spawning on begin play?
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	bool bSpawnOnBeginPlay = true;

	/**
	 * @brief Should start spawning on tick?
	 * Likely will not be used, may come in handy for enemies that spawn quickly
	 * (think a swarm of bees)
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	bool bSpawnOnTick = false;

	FTimerHandle EnemySpawnTimerHandle;
};