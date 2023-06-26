// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BsAudioComponentBase.h"
#include "Components/ActorComponent.h"
#include "BsEnemyAudioComponent.generated.h"

class USoundBase;
class ABsEnemyBase;

USTRUCT(BlueprintType)
struct FEnemyAudioData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* HitStunSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* TookDamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Data")
	USoundBase* BarkSound;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsEnemyAudioComponent : public UBsAudioComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBsEnemyAudioComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

	UFUNCTION()
	void OnHitStun();

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnTookDamage();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component", meta = (AllowPrivateAccess = "true"))
	ABsEnemyBase* EnemyOwner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Component", meta = (AllowPrivateAccess = "true"))
	FEnemyAudioData EnemyAudioData;
};