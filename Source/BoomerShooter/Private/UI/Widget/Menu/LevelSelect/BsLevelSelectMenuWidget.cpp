// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Menu/LevelSelect/BsLevelSelectMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/Menu/LevelSelect/BsLevelSelectItemWidget.h"

void UBsLevelSelectMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	CreateLevelSelectItems();
}

void UBsLevelSelectMenuWidget::CreateLevelSelectItems()
{
	if (!LevelSelectItemWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("LevelSelectItemWidgetClass is not set!"));
		return;
	}
	
	for (auto Item : Config.LevelSelectItems)
	{
		// Create Item Widget and add to list.
		if (UBsLevelSelectItemWidget* ItemWidget = CreateWidget<UBsLevelSelectItemWidget>(GetWorld(), LevelSelectItemWidgetClass))
		{
			ItemWidget->SetLevelSelectItem(Item);
			ItemWidget->OnSelected.AddDynamic(this, &UBsLevelSelectMenuWidget::OnLevelSelectItemWidgetSelected);
			LevelItemWidgetCreated(ItemWidget);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create Item Widget!"));
		}
	}
}

void UBsLevelSelectMenuWidget::LevelItemWidgetCreated_Implementation(UBsLevelSelectItemWidget* LevelSelectItemWidget)
{
}

void UBsLevelSelectMenuWidget::OnLevelSelectItemWidgetSelected(const FBsLevelSelectItem& LevelSelectItem)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LevelSelectItem.Level);
}
