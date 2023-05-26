// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BsHealthAmountWidget.h"

#include "Component/BsHealthComponent.h"

void UBsHealthAmountWidget::BindToHealthComponent(UBsHealthComponent* InHealthComponent)
{
	this->HealthComponent = InHealthComponent;
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &UBsHealthAmountWidget::RefreshHealthAmount);
		RefreshHealthAmount();
	}
}

void UBsHealthAmountWidget::RefreshHealthAmount()
{
	if (HealthComponent)
	{
		HealthAmount = HealthComponent->GetCurrentHealth();
		MaxHealthAmount = HealthComponent->GetMaxHealth();
		UpdateWidget();
	}
}

void UBsHealthAmountWidget::UpdateWidget_Implementation()
{
}
