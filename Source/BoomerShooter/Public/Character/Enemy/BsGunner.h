// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BsRangedEnemy.h"
#include "BsGunner.generated.h"

UCLASS()
class BOOMERSHOOTER_API ABsGunner : public ABsRangedEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABsGunner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

};