// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsFlyingRangedEnemy.h"

#include "GameFramework/CharacterMovementComponent.h"

ABsFlyingRangedEnemy::ABsFlyingRangedEnemy()
{
	
}

void ABsFlyingRangedEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (UCharacterMovementComponent* UCharacterMovementComponent = GetCharacterMovement())
	{
		UCharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}

bool ABsFlyingRangedEnemy::LocationInRange(const FVector& Location) const
{
	return Super::LocationInRange(Location) && FMath::Abs(GetActorLocation().Z - Location.Z) <= MaxAttackZRange;
}
