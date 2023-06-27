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
	 * @brief Virtual function for spawning enemies
	 **/
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	virtual ABsEnemyBase* SpawnEnemy();
	
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

	// TODO: Create functionality to prevent enemies from spawning inside of objects.
};