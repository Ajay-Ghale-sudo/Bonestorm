// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BsLevelSelectMenuWidget.generated.h"

class UBsLevelSelectItemWidget;


USTRUCT(BlueprintType)
struct FBsLevelSelectItem
{
	GENERATED_BODY()

	/**
	 * @brief Name of the level.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Select Item")
	FText Name;

	/**
	 * @brief Image of the level.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Select Item")
	TSoftObjectPtr<UTexture2D> Image;

	/**
	 * @brief Level to load.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Select Item")
	TSoftObjectPtr<UWorld> Level;
};

USTRUCT(BlueprintType)
struct FBsLevelSelectMenuWidgetConfig
{
	GENERATED_BODY()

	/**
	 * @brief Array of Level Select Items.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Select Item")
	TArray<FBsLevelSelectItem> LevelSelectItems;	
};

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsLevelSelectMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	/**
	 * @brief Creates UBsLevelSelectWidget's based on Config.
	 */
	void CreateLevelSelectItems();

	/**
	 * @brief Called when a UBsLevelSelectWidget is created.
	 * @param LevelSelectItemWidget The created UBsLevelSelectWidget.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void LevelItemWidgetCreated(UBsLevelSelectItemWidget* LevelSelectItemWidget);

	/**
	 * @brief Called when a UBsLevelSelectWidget is selected.
	 * @param LevelSelectItem The selected UBsLevelSelectWidget's LevelSelectItem.
	 */
	UFUNCTION()
	void OnLevelSelectItemWidgetSelected(const FBsLevelSelectItem& LevelSelectItem);
	
protected:
	/**
	 * @brief Config for the Level Select Menu.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Select Item")
	FBsLevelSelectMenuWidgetConfig Config;

	/**
	 * @brief Class of the Level Select Item Widget.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Select Item")
	TSubclassOf<UBsLevelSelectItemWidget> LevelSelectItemWidgetClass;
};
