#pragma once

#include "CoreMinimal.h"
#include "Component/Audio/BsAudioComponentBase.h"
#include "BsCharacterAudioComponent.generated.h"

class ABsCharacter;
class UAudioComponent;

USTRUCT(BlueprintType)
struct FCharacterAudioData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* DashSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* SlideSound;

	UPROPERTY()
	UAudioComponent* SlideAudioComponent;
};

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsCharacterAudioComponent : public UBsAudioComponentBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

	UFUNCTION()
	void OnDash();

	UFUNCTION()
	void OnSlideStart();

	UFUNCTION()
	void OnSlideStop();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component", meta = (AllowPrivateAccess = "true"))
	ABsCharacter* CharacterOwner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Component", meta = (AllowPrivateAccess = "true"))
	FCharacterAudioData CharacterAudioData;
};