﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BsCharacter.generated.h"


class UInputAction;

// A structure to hold the default input configuration for the game.
USTRUCT(BlueprintType)
struct FBsInputConfig
{
	GENERATED_BODY()

	// The default context for the input mapping.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultInputContext;

	// The input action for moving the character.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// The input action for changing the character's view direction.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// The input action for making the character jump.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// The input action for dashing.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;
	
};

// A structure to hold the configuration for the dash mechanic.
USTRUCT(BlueprintType)
struct FBsDashConfig
{
	GENERATED_BODY()

	// The base strength of the dash.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float BaseDashStrength = 2000.f;

	// The strength of the dash when the character is on the ground.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float GroundDashStrength = 4000.f;

	// The cooldown time between dashes.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float DashCooldown = 0.5f;

	// A timer handle for the dash cooldown.
	FTimerHandle DashCooldownTimerHandle;

	// A flag to indicate if dashing is currently enabled.
	bool bDashEnabled = true;
	
};


UCLASS()
class BOOMERSHOOTER_API ABsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABsCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/**
	 * @briefHandles the Move input action.
	 * @param Value	The value of the input action.
	 */
	void Move(const FInputActionValue& Value);

	/**
	 * @briefHandles the Look input action.
	 * @param Value	The value of the input action.
	 */
	void Look(const FInputActionValue& Value);

	/**
	 * @brief Handles the Jump input action.
	 */
	virtual void Jump() override;

	/**
	 * @brief Handles the Dash input action.
	 */
	void Dash();

	/**
	 * @brief Enables dashing.
	 */
	void EnableDash();

protected:
	/**
	 * @brief The default input configuration for the Character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FBsInputConfig InputConfig;

	/**
	 * @brief The dash configuration for the Character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	FBsDashConfig DashConfig;

public:

};
