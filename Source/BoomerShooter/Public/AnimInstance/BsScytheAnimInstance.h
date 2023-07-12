// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "BsScytheAnimInstance.generated.h"

class ABsScythe;
/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsScytheAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void StartParry();

	UFUNCTION(BlueprintCallable)
	void StopParry();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Scythe)
	ABsScythe* Scythe;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Scythe)
	bool bMeleeMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Scythe)
	bool bRangedMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Scythe)
	bool bThrown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Scythe)
	bool bAttachedToGrapplePoint;
};