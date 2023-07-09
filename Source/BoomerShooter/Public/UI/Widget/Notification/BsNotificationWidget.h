#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BsNotificationWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void ShowNotification(const FText& Text);
};