// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsFlyingEnemy.h"

#include "GameFramework/CharacterMovementComponent.h"

ABsFlyingEnemy::ABsFlyingEnemy()
{
	
}

void ABsFlyingEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (UCharacterMovementComponent* UCharacterMovementComponent = GetCharacterMovement())
	{
		UCharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}
