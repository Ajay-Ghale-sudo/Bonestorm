// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BsEnemyBase.h"
#include "BsSkeleton.generated.h"

UCLASS()
class BOOMERSHOOTER_API ABsSkeleton : public ABsEnemyBase
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Sets default values for this character's properties
	ABsSkeleton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TriggerRagdoll();

public:


	
	
};
