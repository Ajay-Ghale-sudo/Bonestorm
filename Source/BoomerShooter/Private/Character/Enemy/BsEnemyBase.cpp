// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsEnemyBase.h"

#include "Component/BsHealthComponent.h"


// Sets default values
ABsEnemyBase::ABsEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UBsHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABsEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ABsEnemyBase::Die);
	}
	
}

void ABsEnemyBase::Die()
{
}

// Called every frame
void ABsEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



