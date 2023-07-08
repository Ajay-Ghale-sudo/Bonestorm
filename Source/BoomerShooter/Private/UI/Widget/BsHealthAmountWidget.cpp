#include "UI/Widget/BsHealthAmountWidget.h"
#include "Component/BsHealthComponent.h"

void UBsHealthAmountWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (const APawn* OwningPawn = GetOwningPlayerPawn())
	{
		BindToHealthComponent(OwningPawn->FindComponentByClass<UBsHealthComponent>());
	}
}

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