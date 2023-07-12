#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BsWorldStatTracker.generated.h"

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsWorldStatTracker : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
};