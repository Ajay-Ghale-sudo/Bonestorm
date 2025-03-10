#pragma once

#include "BsSwayWidget.h"
#include "BsHealthAmountWidget.generated.h"

class UBsHealthComponent;

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsHealthAmountWidget : public UBsSwayWidget
{
	GENERATED_BODY()

public:
	void BindToHealthComponent(UBsHealthComponent* InHealthComponent);

	UFUNCTION()
	void RefreshHealthAmount();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void UpdateWidget();

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UBsHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float HealthAmount = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float MaxHealthAmount = 1.f;
};