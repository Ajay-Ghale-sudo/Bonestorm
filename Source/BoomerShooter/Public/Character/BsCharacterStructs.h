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
	UInputAction* AttackModeSwitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlideAction;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashCost = 33.0f;

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
	float DashChargeRate = 0.005f;

	// Base dash recharge amount (later multiplied by DeltaTime)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashChargeAmount = 25.f;

	

	FTimerHandle DashChargeTimerHandle;
	
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