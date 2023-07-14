#include "UI/Widget/Indicator/BsLowHealthIndicatorWidget.h"
#include "Component/BsHealthComponent.h"

void UBsLowHealthIndicatorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (const APawn* OwningPawn = GetOwningPlayerPawn())
	{
		BindToHealthComponent(OwningPawn->FindComponentByClass<UBsHealthComponent>());
	}
}

bool UBsLowHealthIndicatorWidget::IsOwnerLowHealth() const
{
	return HealthComponent.IsValid() && HealthComponent->IsLowHealth();
}

void UBsLowHealthIndicatorWidget::BindToHealthComponent(UBsHealthComponent* InHealthComponent)
{

	if (HealthComponent.IsValid())
	{
		HealthComponent->OnLowHealth.RemoveDynamic(this, &UBsLowHealthIndicatorWidget::OwnerLowHealth);
	}

	HealthComponent = InHealthComponent;
	if (InHealthComponent)
	{
		HealthComponent->OnLowHealth.AddDynamic(this, &UBsLowHealthIndicatorWidget::OwnerLowHealth);
	}
}

void UBsLowHealthIndicatorWidget::OwnerLowHealth_Implementation()
{
	
}