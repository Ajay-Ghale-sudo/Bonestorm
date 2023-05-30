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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FActorSpawnParameters SpawnParams;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<ABsEnemyBase> SpawnedEnemy;
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void Spawn();
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
