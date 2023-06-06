// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsGunner.h"

// Sets default values
ABsGunner::ABsGunner()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABsGunner::BeginPlay()
{
	Super::BeginPlay();
}