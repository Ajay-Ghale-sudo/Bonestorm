#include "UI/Widget/Indicator/BsDamageIndicatorWidget.h"
#include "Component/BsHealthComponent.h"

void UBsDamageIndicatorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (const APawn* OwningPawn = GetOwningPlayerPawn())
	{
		BindToHealthComponent(OwningPawn->FindComponentByClass<UBsHealthComponent>());
	}
}

void UBsDamageIndicatorWidget::OwnerDamagedBy_Implementation(AActor* DamagedBy)
{
}

void UBsDamageIndicatorWidget::BindToHealthComponent(UBsHealthComponent* HealthComponent)
{
	if (HealthComponent)
	{
		HealthComponent->OnDamagedBy.AddUObject(this, &UBsDamageIndicatorWidget::OwnerDamagedBy);
	}
}