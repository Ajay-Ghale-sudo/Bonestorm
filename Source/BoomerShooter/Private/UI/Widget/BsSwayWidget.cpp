// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BsSwayWidget.h"
#include "Character/BsCharacter.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"

void UBsSwayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AssignCanvasPanelSlot();
	BindToCharacter();
}

void UBsSwayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	ApplyWidgetSway(InDeltaTime);
}

void UBsSwayWidget::BindToCharacter()
{
	if (ABsCharacter* Character = Cast<ABsCharacter>(GetOwningPlayerPawn()))
	{
		Character->OnCharacterSway.AddUObject(this, &ThisClass::ReceiveCharacterMovement);
		Character->OnHeadBob.AddDynamic(this, &ThisClass::ApplyHeadBob);
		Character->OnDash.AddUObject(this, &ThisClass::EnableZoom);
		Character->OnDashFinished.AddUObject(this, &ThisClass::DisableZoom);
		Character->OnSlideStart.AddUObject(this, &ThisClass::EnableZoom);
		Character->OnSlideStop.AddUObject(this, &ThisClass::DisableZoom);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UBsSwayWidget::BindToCharacter: Character is null"));
	}
}

void UBsSwayWidget::AssignCanvasPanelSlot()
{
	if (SwayContainer)
	{
		SwayConfig.TargetPanelSlot = Cast<UCanvasPanelSlot>(SwayContainer->Slot);
		if (!SwayConfig.TargetPanelSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("UBsSwayWidget::AssignCanvasPanelSlot: SwayContainer doesn't have a CanvasPanelSlot"));
			return;
		}
		SwayConfig.InitialPosition = SwayConfig.TargetPanelSlot->GetPosition();
		SwayConfig.TargetPosition = SwayConfig.InitialPosition;
	}
}

void UBsSwayWidget::ApplyWidgetSway(const float DeltaTime)
{
	if (!SwayConfig.TargetPanelSlot || !SwayContainer) return;

	SwayConfig.UpdateTargetPosition();

	// Apply Movement
	const FVector2D NewPosition = SwayConfig.GetNewPosition(SwayConfig.TargetPanelSlot->GetPosition(), DeltaTime);
	SwayConfig.TargetPanelSlot->SetPosition(NewPosition);

	// Apply Zoom
	float NewScale = SwayConfig.LastVerticalAmount * SwayConfig.VerticalMovementZoomMultiplier;
	NewScale *= SwayConfig.bApplyBurstZoom ? SwayConfig.BurstZoomMultiplier : 1.f;
	NewScale += 1.f; // Offset to make sure it's always bigger than 0
	const FVector2D NewSize = FMath::Vector2DInterpTo(
		SwayContainer->GetRenderTransform().Scale,
		FVector2D(NewScale),
		DeltaTime,
		SwayConfig.SwaySpeed
	);
	SwayContainer->SetRenderScale(NewSize);

	// Reset Movement so if there isn't input by next frame it will go back to initial position
	SwayConfig.ClearLastMovement();
}


void UBsSwayWidget::ReceiveCharacterMovement(float HorizontalAmount, float VerticalAmount)
{
	SwayConfig.LastHorizontalAmount = HorizontalAmount;
	SwayConfig.LastVerticalAmount = VerticalAmount;
}

void UBsSwayWidget::ApplyHeadBob(const float Amount)
{
	SwayConfig.HeadBobOffset = Amount;
}

void UBsSwayWidget::EnableZoom()
{
	SwayConfig.bApplyBurstZoom = true;
}

void UBsSwayWidget::DisableZoom()
{
	SwayConfig.bApplyBurstZoom = false;
}