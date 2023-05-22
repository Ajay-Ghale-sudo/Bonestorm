// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BsCharacterStructs.h"
#include "BsCharacter.generated.h"

class UBsInventoryComponent;
class ABsWeaponBase;
class UCameraComponent;
class UInputAction;

DECLARE_MULTICAST_DELEGATE(FBsCharacterEvent);

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

	/**
	 * @brief Sets the Character's Weapon to the specified Weapon.
	 * @param InWeapon New Weapon to equip.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeapon(ABsWeaponBase* InWeapon);

public:
	FBsCharacterEvent OnDashAmountChanged;
	
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

	/**
	 * @brief Add Dash Charge
	 */
	void AddDashCharge();

	/**
	 * @brief Enables grapple
	 */
	void StartGrapple();
	
	/**
	 * @brief Disabled grapple
	 */
	void StopGrapple();

	/**
	 * @brief Attacks with the weapon.
	 */
	void Attack();

	/**
	 * @brief Uses secondary attack with the weapon
	 */
	void SecondaryAttack();
	
	/**
	 * @brief Change the Weapon Mode to the next available.
	 */
	void NextWeaponMode();

	/**
	 * @brief Attempt to interact with an object in front of the character.
	 */
	void Interact();

	/**
	 * @brief Attempt to Start Sliding.
	 */
	void StartSliding();

	/**
	 * @brief Stop Sliding if the character is sliding.
	 */
	void StopSliding();

	/**
	 * @brief This function handles the per-frame behavior during sliding.
	 *
	 * This function is called every frame and it performs several checks and operations related to sliding.
	 * If the character is currently sliding but their velocity is below the velocity needed to stop the slide,
	 * the character will stop sliding. Additionally, the function adjusts the character's capsule half-height
	 * to smoothly transition between the pre-slide height and the sliding height.
	 *
	 * @param DeltaTime The time passed since the last frame.
	 */
	void SlideTick(float DeltaTime);
	
protected:
	/**
	 * @brief The default input configuration for the Character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FBsInputConfig InputConfig;

	/**
	 * @brief The dash configuration for the Character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	FBsDashConfig DashConfig;

	/**
	 * @brief The slide configuration for the Character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Slide")
	FBsSlideConfig SlideConfig;
	
	/**
	 * @brief Interaction configuration for the Character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interact)
	FBsInteractConfig InteractConfig;

	/**
	 * @brief The Camera Component of the Character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComponent;

	/**
	 * @brief Inventory Component of the Character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	UBsInventoryComponent* InventoryComponent;

	/**
	 * @brief Currently Equipped Weapon.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	ABsWeaponBase* Weapon;
	
	bool bGrappling = false;
public:
	FORCEINLINE int32 GetDashAmount() const { return DashConfig.DashCharges; }
};
