#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BsPlayerController.generated.h"

UCLASS()
class BOOMERSHOOTER_API ABsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsPlayerController();

	virtual bool SetPause(bool bPause, FCanUnpause CanUnpauseDelegate) override;

	UFUNCTION(BlueprintCallable)
	void Quit();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnPause();

	UFUNCTION()
	void OnUnpause();
};