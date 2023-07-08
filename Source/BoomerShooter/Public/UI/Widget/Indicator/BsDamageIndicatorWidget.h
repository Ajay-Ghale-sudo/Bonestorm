#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BsDamageIndicatorWidget.generated.h"

class UBsHealthComponent;

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsDamageIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindToHealthComponent(UBsHealthComponent* HealthComponent);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageIndicator")
	void OwnerDamagedBy(AActor* DamagedBy);
	
protected:
	virtual void NativeConstruct() override;
};