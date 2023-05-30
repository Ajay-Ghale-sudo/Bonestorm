// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/BsEnemyBase.h"
#include "GameFramework/Actor.h"
#include "BsEnemySpawner.generated.h"

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
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SpawnTimer();
	
	/**
	 * @brief Function for spawning enemies
	 **/
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SpawnEnemy();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<ABsEnemyBase> SpawnedEnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float SpawnInterval = 1.f;

	FTimerHandle EnemySpawnTimer;
};