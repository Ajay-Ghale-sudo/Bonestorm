#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BsStartMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsStartMenuWidgetEvent);

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsStartMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FBsStartMenuWidgetEvent OnStart;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FBsStartMenuWidgetEvent OnQuit;
};