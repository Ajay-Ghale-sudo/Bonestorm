// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BsAudioComponentBase.generated.h"

USTRUCT(BlueprintType)
struct FBsAudioData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundAttenuation* AttenuationSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundConcurrency* ConcurrencySettings;
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsAudioComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBsAudioComponentBase();
	
	virtual UAudioComponent* PlaySound(USoundBase* Sound);
	virtual UAudioComponent* PlaySound(USoundBase* Sound, USoundAttenuation* AttenuationSettings, USoundConcurrency* ConcurrencySettings);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void BindEvents();
	virtual void UnbindEvents();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Component", meta = (AllowPrivateAccess = "true"))
	FBsAudioData AudioData;
};