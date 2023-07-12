﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BsCharacterStructs.h"
#include "BsCharacter.generated.h"

class UBsCameraComponent;
class USpringArmComponent;
class UBsInventoryComponent;
class ABsWeaponBase;
class UCameraComponent;
class UInputAction;
class ABsSeveredHeadBase;
class UBsHealthComponent;
class UBsCharacterAudioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsCharacterInputEvent);
DECLARE_MULTICAST_DELEGATE(FBsCharacterEvent);
DECLARE_MULTICAST_DELEGATE_OneParam(FBsCharacterWeaponEvent, ABsWeaponBase*);

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
	virtual void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta) override;

	/**
	 * @brief Sets the Character's Weapon to the specified Weapon.
	 * @param InWeapon New Weapon to equip.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeapon(ABsWeaponBase* InWeapon);

	/**
	 * @brief Toggle between Pause/Unpause
	 */
	UFUNCTION(BlueprintCallable)
	void TogglePause();
	
	/*
	 * @brief Pauses the game.
	 */
	UFUNCTION(BlueprintCallable)
	void Pause();

	/**
	 * @brief Unpauses the game.
	 */
	UFUNCTION(BlueprintCallable)
	void Unpause();

	UFUNCTION()
	void StartHitStop();

	UFUNCTION()
	void StopHitStop();

	virtual void FellOutOfWorld(const UDamageType& DmgType) override;

public:
	FBsCharacterEvent OnDashAmountChanged;
	FBsCharacterEvent OnDashEnabledChanged;
	FBsCharacterEvent OnDash;

	FBsCharacterEvent OnJump;
	FBsCharacterEvent OnDashJump;
	FBsCharacterEvent OnSlideJump;
	
	FBsCharacterEvent OnSlideStart;
	FBsCharacterEvent OnSlideStop;

	FBsCharacterEvent OnArenaStarted;
	FBsCharacterEvent OnArenaEnded;

	UPROPERTY(BlueprintReadOnly, BlueprintAssignable)
	FBsCharacterInputEvent OnPaused;

	UPROPERTY(BlueprintReadOnly, BlueprintAssignable)
	FBsCharacterInputEvent OnUnpaused;

	FBsCharacterWeaponEvent OnWeaponChanged;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief Handles player taking damage with blocks and parries.
	 */
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
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

	virtual void OnJumped_Implementation() override;

	/**
	 * @brief Handles the Dash input action.
	 */
	void Dash();

	/**
	 * @brief Stop Dashing.
	 */
	void StopDashing();

	/**
	 * @brief Revert to pre dash state.
	 */
	void FinishDashing();

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
	 * @brief Temp, refund dash charge
	 */
	void RefundDashCharge();

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
	void OnGrappleDetached();

	UFUNCTION()
	void PullGrapple(FVector Vector);
	
	/**
	 * @brief Attacks with the weapon.
	 */
	void Attack();
	
	/**
	 * @brief Start blocking with a weapon.
	 */
	void StartBlock();

	/**
	 * @brief Stops blocking with a weapon.
	 */
	void StopBlock();

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
	 * @brief Detach head from weapon
	 */
	void DetachHead();

	/**
	 * @brief Consume head on weapon
	 */
	void ConsumeHead();

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
	 * @brief Start coyote time, allowing for an extra jump while in midair.
	 */
	UFUNCTION()
	void StartCoyoteTime();

	/**
	 * @brief End coyote time.
	 */
	UFUNCTION()
	void EndCoyoteTime();

	UFUNCTION()
	void OnParry();

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
	 * @brief Movement configuration for the Character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FBsMovementConfig MovementConfig;
	
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
	 * @brief Hit Stop configuration for the Character.
	 */
	FBsHitStopConfig HitStopConfig;
	
	/**
	 * @brief The Camera Component of the Character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	UBsCameraComponent* CameraComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio)
	UBsCharacterAudioComponent* AudioComponent;

	/**
	 * @brief Currently Equipped Weapon.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Weapon")
	ABsWeaponBase* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|Weapon")
	USpringArmComponent* WeaponSpringArmComponent;
	
	bool bGrappleAttached = false;
	bool bBeingPulledToGrapple = false;
	bool bAlive = true;
public:

	FORCEINLINE int32 GetDashAmount() const { return DashConfig.DashCurrentAmount; }
	FORCEINLINE bool GetDashEnabled() const { return DashConfig.bDashEnabled; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsAlive() const { return bAlive; }
	FORCEINLINE UBsHealthComponent* GetHealthComponent() const { return HealthComponent; }
	FORCEINLINE UBsInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
};