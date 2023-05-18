// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/BsSkeletonAnimInstance.h"

#include "Character/Enemy/BsSkeleton.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

void UBsSkeletonAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (ABsSkeleton* Owner = Cast<ABsSkeleton>(TryGetPawnOwner()))
	{
		Skeleton = Owner;
	}
}

void UBsSkeletonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	Skeleton = Skeleton ? Skeleton : Cast<ABsSkeleton>(TryGetPawnOwner());
	if (!Skeleton) return;

	bIsFalling = Skeleton->GetMovementComponent()->IsFalling();
	bIsAccelerating = Skeleton->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0;

	FVector Velocity = Skeleton->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	bIsAlive = Skeleton->GetIsAlive();
}
