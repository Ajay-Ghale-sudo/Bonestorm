﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsSkeleton.h"


// Sets default values
ABsSkeleton::ABsSkeleton()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABsSkeleton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABsSkeleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
