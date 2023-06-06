// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BsRangedEnemy.h"
#include "BsFlyingRangedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsFlyingRangedEnemy : public ABsRangedEnemy
{
	GENERATED_BODY()

public:
	ABsFlyingRangedEnemy();

protected:
	virtual void BeginPlay() override;
	
};
