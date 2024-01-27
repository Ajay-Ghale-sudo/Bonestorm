// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BsSwayWidget.generated.h"

class UCanvasPanelSlot;
class UPanelWidget;


USTRUCT(BlueprintType)
struct FBsWidgetSwayConfig
{
	GENERATED_BODY()

	/**
	 * @brief Amount of Horizontal Sway from the Character.
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Widget|Sway")
	float LastHorizontalAmount = 0.f;

	/**
	 * @brief Amount of Vertical Sway from the Character.
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Widget|Sway")
	float LastVerticalAmount = 0.f;

	/**
	 * @brief How much the widget should sway.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Sway")
	float SwayMultiplier = 20.f;

	/**
	 * @brief Whether or not to apply the burst zoom multiplier to the sway.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget|Sway")
	bool bApplyBurstZoom = false;

	/**
	 * @brief The multiplier to apply to the sway when the player is moving Forward/Backwards.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Sway")
	float VerticalMovementZoomMultiplier = .1f;

	/**
	 * @brief The multiplier to apply to the sway when the player burst movement occurs. (Dash | Slide).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Sway")
	float BurstZoomMultiplier = 2.f;

	/**
	 * @brief The slot of the widget to sway.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Sway")
	TObjectPtr<UCanvasPanelSlot> TargetPanelSlot;

	/**
	 * @brief The speed at which the widget should sway.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Sway")
	float SwaySpeed = 5.f;

	/**
	 * @brief The amount of head bob sway to apply to the widget.
	 */
	float HeadBobOffset = 0.f;

	/**
	 * @brief How much the head bob should affect the widget sway.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Sway")
	float HeadBobMultiplier = 25.f;

	/**
	 * @brief The initial position of the widget slot.
	 */
	FVector2D InitialPosition;

	/**
	 * @brief The target position of the widget slot.
	 */
	FVector2D TargetPosition;

	/**
	 * @brief Calculates and sets the target position of the widget slot.
	 */
	FORCEINLINE void UpdateTargetPosition()
	{
		TargetPosition.X = InitialPosition.X + (LastHorizontalAmount * -SwayMultiplier * (bApplyBurstZoom ? BurstZoomMultiplier : 1.f));
		TargetPosition.Y = InitialPosition.Y + (HeadBobOffset * HeadBobMultiplier);
	}

	/**
	 * @brief Gets the new position of the widget slot. Interpolates between the current position and the target position.
	 */
	FORCEINLINE FVector2D GetNewPosition(const FVector2D CurrentPosition, const float DeltaTime) const
	{
		return FMath::Vector2DInterpTo(CurrentPosition, TargetPosition, DeltaTime, SwaySpeed);
	}

	/**
	 * @brief Clears the last movement values. (Horizontal/Vertical).
	 */
	FORCEINLINE void ClearLastMovement()
	{
		LastHorizontalAmount = 0.f;
		LastVerticalAmount = 0.f;
	}
};

/**
 * A widget that can be swayed by the player's movement.
 */
UCLASS()
class BOOMERSHOOTER_API UBsSwayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Update the widgets Horizontal/Vertical movement values. Typically these are normalized values based off input.
	 * @param HorizontalAmount How much the player is moving horizontally.
	 * @param VerticalAmount How much the player is moving vertically.
	 */
	UFUNCTION()
	void ReceiveCharacterMovement(float HorizontalAmount, float VerticalAmount);

	/**
	 * @brief Set the amount of head bob to apply to the widget.
	 * @param Amount The amount of head bob to apply to the widget.
	 */
	UFUNCTION()
	void ApplyHeadBob(const float Amount);

	/**
	 * @brief Enable the zoom multiplier for the widget sway.
	 */
	UFUNCTION()
	void EnableZoom();

	/**
	 * @brief Disable the zoom multiplier for the widget sway.
	 */
	UFUNCTION()
	void DisableZoom();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/**
	 * @brief Assign the canvas panel slot to the widget from the SwayContainer.
	 */
	virtual void AssignCanvasPanelSlot();

	/**
	 * @brief Bind the widget to the character events.
	 */
	virtual void BindToCharacter();

	/**
	 * @brief Apply the widget sway.
	 * @param DeltaTime The time since the last tick.
	 */
	UFUNCTION()
	void ApplyWidgetSway(const float DeltaTime);

protected:
	/**
	 * @brief The widget sway configuration.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Sway")
	FBsWidgetSwayConfig SwayConfig;

	/**
	 * @brief The PanelWidget which Slot will be used for swaying.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Sway", meta = (BindWidget))
	UPanelWidget* SwayContainer;
};