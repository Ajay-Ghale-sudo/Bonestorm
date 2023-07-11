#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BsSeveredHeadWidget.generated.h"

class ABsSeveredHeadBase;

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsSeveredHeadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSeveredHead(ABsSeveredHeadBase* InSeveredHead);

	UFUNCTION()
	void ClearSeveredHead();

protected:
	UFUNCTION()
	void HeadChargeChanged();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateHeadCharge(const float NewCharge);

	UFUNCTION(BlueprintNativeEvent)
	void HeadAttached();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Severed Head Widget")
	ABsSeveredHeadBase* SeveredHead;
};