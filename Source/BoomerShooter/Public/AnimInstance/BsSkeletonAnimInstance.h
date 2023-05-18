// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "BsSkeletonAnimInstance.generated.h"

class ABsSkeleton;
/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsSkeletonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skeleton)
	ABsSkeleton* Skeleton;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skeleton)
	bool bIsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skeleton)
	bool bIsAccelerating;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skeleton)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skeleton)
	bool bIsAlive;
};
