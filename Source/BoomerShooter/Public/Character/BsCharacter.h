// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BsCharacterStructs.h"
#include "BsCharacter.generated.h"

class USpringArmComponent;
class UBsInventoryComponent;
class ABsWeaponBase;
class UCameraComponent;
class UInputAction;
class ABsSeveredHeadBase;
class UBsHealthComponent;

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
	FBsCharacterEvent OnDashEnabledChanged;
	
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
	 * @brief Process and apply Dash Movement.
	 * @param DeltaTime	The time passed since the last frame.
	 */
	void DashTick(const float DeltaTime);

	/**
	 * @brief Enables dashing.
	 */
	void EnableDash();

	/**
	 * @brief Add Dash Charge
	 */
	void AddDashCharge();

	/**
	 * @brief Checks if the player can dash
	 */
	bool CanDash();

	/**
	 * @brief Enables grapple
	 */
	UFUNCTION()
	void StartGrapple();
	
	/**
	 * @brief Disabled grapple
	 */
	UFUNCTION()
	void StopGrapple();

	UFUNCTION()
	void PullGrapple(FVector Vector);
	
	/**
	 * @brief Attacks with the weapon.
	 */
	void Attack();

	/**
	 * @brief Uses secondary attack with the weapon
	 */
	void SecondaryAttack();

    /**
     * @brief Throws the current weapon.
     */
    void ThrowWeapon();
	
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

	UFUNCTION()
	void Die();


	UFUNCTION()
	void OnSeveredHeadPickup(ABsSeveredHeadBase* Head);

	UFUNCTION()
	void GrabCurrentWeapon();
	
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
     * @brief Player Health Component
     */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStatus)
	UBsHealthComponent* HealthComponent;

	/**
	 * @brief Currently Equipped Weapon.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Weapon")
	ABsWeaponBase* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|Weapon")
	USpringArmComponent* WeaponSpringArmComponent;
	
	bool bGrappling = false;
	bool bAlive = true;
public:

	FORCEINLINE int32 GetDashAmount() const { return DashConfig.DashCurrentAmount; }
	FORCEINLINE bool GetDashEnabled() const { return DashConfig.bDashEnabled; }
};
