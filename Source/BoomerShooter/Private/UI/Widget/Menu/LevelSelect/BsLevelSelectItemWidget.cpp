// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widget/Menu/LevelSelect/BsLevelSelectItemWidget.h"

void UBsLevelSelectItemWidget::SetLevelSelectItem(const FBsLevelSelectItem& InLevelSelectItem)
{
	LevelSelectItem = InLevelSelectItem;
	RefreshWidget();
}

void UBsLevelSelectItemWidget::RefreshWidget_Implementation()
{
}

void UBsLevelSelectItemWidget::Hovered_Implementation(bool bHovered)
{
}


void UBsLevelSelectItemWidget::Selected()
{
	OnSelected.Broadcast(LevelSelectItem);
}

void UBsLevelSelectItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Hovered(true);
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UBsLevelSelectItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Hovered(false);
	Super::NativeOnMouseLeave(InMouseEvent);
}

FReply UBsLevelSelectItemWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Selected();
	return FReply::Handled();
}
