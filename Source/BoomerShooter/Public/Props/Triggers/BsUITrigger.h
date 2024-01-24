// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "BsUITrigger.generated.h"

class UBoxComponent;

UCLASS()
class BOOMERSHOOTER_API ABsUITrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsUITrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers")
	UBoxComponent* TriggerBox;

	/**
	 * @brief The gameplay tag that will be used with the UI event system.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers")
	FGameplayTag UIGameplayTag;

	/**
	 * @brief If this is true, the trigger will end the event when the player leaves the trigger box.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers")
	bool bEndEventOnOverlapEnd = true;
};
