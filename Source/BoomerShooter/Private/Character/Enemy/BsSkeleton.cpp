// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy/BsSkeleton.h"

// Sets default values
ABsSkeleton::ABsSkeleton()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABsSkeleton::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABsSkeleton::Die()
{
	Super::Die();
}

void ABsSkeleton::Attack()
{
	Super::Attack();
}

void ABsSkeleton::MeleeAttack()
{
	if (!CanAttack()) return;

	PlayMontage(MeleeAttackMontage);
}