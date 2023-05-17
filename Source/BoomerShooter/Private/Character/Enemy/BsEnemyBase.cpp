// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsEnemyBase.h"


// Sets default values
ABsEnemyBase::ABsEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABsEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABsEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



