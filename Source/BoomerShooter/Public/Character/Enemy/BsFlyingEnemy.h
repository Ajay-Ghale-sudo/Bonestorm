// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Enemy/BsEnemyBase.h"
#include "BsFlyingEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsFlyingEnemy : public ABsEnemyBase
{
	GENERATED_BODY()

public:
	ABsFlyingEnemy();

protected:
	virtual void BeginPlay() override;
	
};
