// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BsLevelSelectMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "BsLevelSelectItemWidget.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBsLevelSelectItemWidgetEvent, const FBsLevelSelectItem&, LevelSelectItem);

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsLevelSelectItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the Level Select Item.
	 * @param InLevelSelectItem Level Select Item to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetLevelSelectItem(const FBsLevelSelectItem& InLevelSelectItem);

public:
	/**
	 * @brief Event triggered when the widget is selected.
	 */
	UPROPERTY(BlueprintAssignable)
	FBsLevelSelectItemWidgetEvent OnSelected;
	

protected:
	/**
	 * @brief Refreshes the widget with the current LevelSelectItem.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void RefreshWidget();

	/**
	 * @brief Triggered when the widget is hovered.
	 * @param bHovered True if hovered, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Hovered(bool bHovered);

	/**
	 * @brief Triggered when the widget is selected.
	 */
	virtual void Selected();
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
protected:
	/**
	 * @brief Level Select Item.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FBsLevelSelectItem LevelSelectItem;
};
