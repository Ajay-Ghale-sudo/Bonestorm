#pragma once


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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	bool bDashJumped = false;

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

	// Modifier to change the amount of dash recharged on a per second basis.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashChargeAmountModifier = 0.25f;
	
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

	void FillDashCharge() { DashCurrentAmount = DashMaxAmount; }

	/**
	 * @brief Decreases DashCurrentAmount by DashCost.
	 */
	void DepleteDash() { DashCurrentAmount = FMath::Clamp(DashCurrentAmount - DashCost, DashMinAmount, DashMaxAmount); }

	/**
	 * @brief Decreases DashCurrenAmount by DashCost * JumpDashMultiplier.
	 */
	FORCEINLINE void DepleteJumpDash()
	{
		DashCurrentAmount = FMath::Clamp(DashCurrentAmount - DashCost * JumpDashMultiplier, DashMinAmount, DashMaxAmount);
		bDashJumped = true;
	}

	FORCEINLINE bool CanDashJump() const
	{
		return DashElapsedTime <= DashJumpTimeWindow && DashCurrentAmount - DashCost * JumpDashMultiplier >= 0;
	}
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
	 * @brief LastMovementVector of the Character. Based off input action not relative direction. (I.E Keyboard WASD)
	 */
	FVector2D LastMovementVector = FVector2D::ZeroVector;

	/**
	 * @brief Last Look input vector of the Character. Based off input action not relative rotation. (I.E Mouse movement)
	 */
	FVector2D LastLookVector = FVector2D::ZeroVector;
	
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
};

/**
 * @brief This struct is used to represent the weapon sway configuration.
 */
USTRUCT(BlueprintType)
struct FBsWeaponSwayConfig
{
	GENERATED_BODY()

	/**
	 * @brief The current amount of weapon sway.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	float CurrentWeaponSwayAmount = 0.f;
	
	/**
	 * @brief How much the weapon should sway horizontally when moving.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	float MaxHorizontalWeaponSwayAmount = 3.f;

	/**
	 * @brief How much the weapon should sway vertically when moving.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	float MaxVerticalWeaponSwayAmount = 1.5f;

	/**
	 * @brief Max amount of roll sway.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	float MaxRollWeaponSwayAmount = 15.f;

	/**
	 * @brief Max amount of pitch sway.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	float MaxPitchWeaponSwayAmount = 15.f;

	/**
	 * @brief How fast the weapon should sway when moving.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	float WeaponMoveSwaySpeed = 15.f;

	/**
	 * @brief How fast the weapon should sway when Looking/Turning
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	float WeaponLookSwaySpeed = 5.f;

	/**
	 * @brief The initial location of the weapon. Used to offset the weapon sway.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	FVector InitialWeaponLocation = FVector::ZeroVector;

	/**
	 * @brief The initial rotation of the weapon. Used to offset the weapon sway.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Weapon")
	FRotator InitialWeaponRotation = FRotator::ZeroRotator;

	/**
	 * @brief Calculate the vertical sway amount.
	 * @param DeltaTime The time since last frame
	 * @param CurrentAmount The current value of the sway amount
	 * @param AmountToAdd The amount to add to the sway amount
	 * @return The new sway amount
	 */
	FORCEINLINE float CalculateVerticalSwayAmount(const float DeltaTime, const float CurrentAmount, const float AmountToAdd) const
	{
		return CalculateSwayAmount(DeltaTime, CurrentAmount, AmountToAdd, MaxPitchWeaponSwayAmount, InitialWeaponRotation.Pitch, WeaponLookSwaySpeed);
	}

	/**
	 * @brief Calculate the horizontal sway amount.
	 * @param DeltaTime The time since last frame
	 * @param CurrentAmount The current value of the sway amount
	 * @param AmountToAdd The amount to add to the sway amount
	 * @return The new sway amount
	 */
	FORCEINLINE float CalculateHorizontalSwayAmount(const float DeltaTime, const float CurrentAmount, const float AmountToAdd) const
	{
		return CalculateSwayAmount(DeltaTime, CurrentAmount, AmountToAdd, MaxRollWeaponSwayAmount, InitialWeaponRotation.Roll, WeaponLookSwaySpeed);
	}

	/**
	 * @brief Calculate the sway amount based on the current amount, amount to add, max amount, initial amount and speed.
	 * @param DeltaTime The time since last frame
	 * @param CurrentAmount The current amount of sway
	 * @param AmountToAdd The amount to add to the sway
	 * @param MaxAmount The max amount of sway
	 * @param InitialAmount The initial amount of sway, typically the starting position.
	 * @param Speed The speed of the sway
	 * @return The new sway amount
	 */
	FORCEINLINE float CalculateSwayAmount(const float DeltaTime, const float CurrentAmount, const float AmountToAdd, const float MaxAmount, const float InitialAmount, const float Speed) const
	{
		float NewAmount;
		if (FMath::IsNearlyZero(AmountToAdd) && !FMath::IsNearlyEqual(CurrentAmount, InitialAmount))
		{
			// Lerp back to initial rotation
			NewAmount = InitialAmount;
		} else
		{
			NewAmount =  FMath::Clamp(
				CurrentAmount + AmountToAdd,
				InitialAmount - MaxAmount,
				InitialAmount + MaxAmount
			);
		}

		return FMath::FInterpTo(CurrentAmount, NewAmount, DeltaTime, Speed);
	}
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

USTRUCT(BlueprintType)
struct FBsHeadBobConfig
{
	GENERATED_BODY()
	/**
	 * @brief Initial relative location of the camera to headbob from (set on Begin Play)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Head Bob")
	FVector InitialRelativeLocation;
	/**
	 * @brief Maximum amount of head bob that can be applied
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Head Bob")
	float MaxHeadBobAmount = -55.0f;

	/**
	 * @brief Minimum velocity required to trigger head bob upon falling
	 */
	float MinimumVelocityThreshold = 450.f;
};
