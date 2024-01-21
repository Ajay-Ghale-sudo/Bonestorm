﻿#pragma once


#include "EnhancedInputSubsystems.h"
#include "BsCharacterStructs.generated.h"

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

	// The input action for attacking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SecondaryAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackModeSwitchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlideAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DetachHeadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ConsumeHeadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrappleAction;
	
};

// A structure to hold the configuration for the dash mechanic.
USTRUCT(BlueprintType)
struct FBsDashConfig
{
	GENERATED_BODY()

	// The base strength of the dash.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float BaseDashStrength = 2000.f;

	// The strength of the dash when the character is on the ground.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float GroundDashStrength = 4000.f;

	// The cooldown time between dashes.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashCooldown = 0.5f;

	// A timer handle for the dash cooldown.
	FTimerHandle DashCooldownTimerHandle;

	// A flag to indicate if dashing is currently enabled.
	bool bDashEnabled = true;

	/**
	 * @brief Cost of a Dash.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashCost = 33.0f;

	/**
	 * @brief Modifier applied to DashCost when Jump Dashing.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
 	float JumpDashMultiplier = 0.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashCurrentAmount;

	// Maximum dash "stamina"
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashMaxAmount = 100.f;
	
	// Minimum dash "stamina"
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashMinAmount = 0.f;

	// Dash recharge rate interval
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashChargeRate = 0.05f;

	// Base dash recharge amount per second
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashChargeAmount = 25.f;

	/**
	 * @brief How long to dash for.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashDuration = 0.1f;

	/**
	 * @brief Time spent dashing.
	 */
	float DashElapsedTime = 0.f;

	/**
	 * @brief The time window after dashing to allow a dash jump.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashJumpTimeWindow = 0.09f;

	/**
	 * @brief Timer Handle for Dash Charging
	 */
	FTimerHandle DashChargeTimerHandle;

	/**
	 * @brief If the Character is dashing or not.
	 */
	bool bDashing = false;

	/**
	 * @brief The Direction the Character is dashing in.
	 */
	FVector DashDirection = FVector::ZeroVector;

	/**
	 * @brief The Velocity before the Character starting Dashing.
	 */
	FVector PreDashVelocity = FVector::ZeroVector;

	/**
	 * @brief Adds a DashCost amount to the DashCurrentAmount.
	 */
	void RefundDashCharge() { DashCurrentAmount = FMath::Clamp(DashCurrentAmount + DashCost, DashMinAmount, DashMaxAmount); }

	/**
	 * @brief Decreases DashCurrentAmount by DashCost.
	 */
	void DepleteDash() { DashCurrentAmount = FMath::Clamp(DashCurrentAmount - DashCost, DashMinAmount, DashMaxAmount); }

	/**
	 * @brief Decreases DashCurrenAmount by DashCost * JumpDashMultiplier.
	 */
	void DepleteJumpDash() { DashCurrentAmount = FMath::Clamp(DashCurrentAmount - DashCost * JumpDashMultiplier, DashMinAmount, DashMaxAmount); }
};

USTRUCT(BlueprintType)
struct FBsInteractConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interact)
	float InteractRange = 200.f;	
};

/**
 * @brief This struct is used to represent the sliding configuration in a Blueprint.
 */
USTRUCT(BlueprintType)
struct FBsSlideConfig
{
	GENERATED_BODY()

	/**
	 * @brief The velocity required to start sliding. 
	 * If the character's speed surpasses this value, the character will start to slide.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Slide")
	float VelocityToStartSlide = 1500.f;

	/**
	 * @brief The velocity required to stop sliding. 
	 * If the character's speed falls below this value, the character will stop sliding.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Slide")
	float VelocityToStopSlide = 1200.f;

	/**
	 * @brief The character's ground friction before the slide starts.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Slide")
	float PreSlideGroundFriction = 1.f;

	/**
	 * @brief The friction applied when the character is sliding.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Slide")
	float SlideFriction = 0.1f;

	/**
	 * @brief The strength of the slide, which could be used to determine how far the character slides.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Slide")
	float SlideStrength = 1000.f;

	/**
	 * @brief The strength of a downward impulse when the character is on a slope.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Slide")
	float SlopeSlideStrength = -500.f;

	/**
	 * @brief How far below to check for a floor when the character is on a slope.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Slide")
	float SlopeSlideFloorDistance = 200.f;

	/**
	 * @brief Timer handle used for controlling slide duration.
	 */
	FTimerHandle SlideTimerHandle;

	/**
	 * @brief The character's height before the slide starts.
	 */
	float PreSlideHeight = 88.f;

	/**
	 * @brief The character's height during the slide.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Slide")
	float SlideHeight = 44.f;

	/**
	 * @brief A flag indicating whether the character is currently sliding or not.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Slide")
	bool bSliding = false;
};

/**
 * @brief This struct is used to represent the movement configuration.
 */
USTRUCT(BlueprintType)
struct FBsMovementConfig
{
	GENERATED_BODY()

	/**
	 * @brief If the character has recently walked off a ledge.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Jump")
	bool bInCoyoteTime = false;

	/**
	 * @brief The duration of the coyote time.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Jump")
	float CoyoteTimeDuration = 0.5f;

	/**
	 * @brief Timer handle used for controlling coyote time.
	 */
	FTimerHandle CoyoteTimeHandle;

	/**
	 * @brief The current amount of weapon sway.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	float CurrentWeaponSwayAmount = 0.f;
	
	/**
	 * @brief How much the weapon should sway when moving.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	float MaxWeaponSwayAmount = 3.f;

	/**
	 * @brief How fast the weapon should sway when moving.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	float WeaponSwaySpeed = 15.f;

	/**
	 * @brief The initial location of the weapon. Used to offset the weapon sway.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	FVector InitialWeaponLocation = FVector::ZeroVector;
};

// A structure to hold the configuration for the hit stop mechanic.
USTRUCT(BlueprintType)
struct FBsHitStopConfig
{
	GENERATED_BODY()

	/**
	 * @brief How long for the HItStop to last. Is relative to the HitStopTimeDilation.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float HitStopDuration = 0.001f;

	/**
	 * @brief The time dilation to apply when hit stop is active.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float HitStopTimeDilation = 0.01f;
	
	FTimerHandle HitStopTimerHandle;
};
