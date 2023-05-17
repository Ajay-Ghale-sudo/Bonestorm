﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BsEnemyBase.generated.h"

class UBsHealthComponent;
UCLASS()
class BOOMERSHOOTER_API ABsEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABsEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Die();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UBsHealthComponent* HealthComponent;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
