// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BsBaseTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBsBaseTriggerEvent, AActor*, TriggeredActor);

class UBoxComponent;
UCLASS()
class BOOMERSHOOTER_API ABsBaseTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsBaseTrigger();

	/**
	 * @brief Reset the trigger to be able to be triggered again. Called initially, can be used with bManualReset to reset the trigger manually.
	 */
	void ResetTrigger();

public:
	/**
	 * @brief Called when the trigger is activated.
	 */
	UPROPERTY(BlueprintAssignable)
	FBsBaseTriggerEvent OnTriggerStart;

	/**
	 * @brief Called when the trigger is deactivated.
	 */
	UPROPERTY(BlueprintAssignable)
	FBsBaseTriggerEvent OnTriggerEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief Called when the trigger is activated by an Actor.
	 * @param TriggeredActor The Actor that triggered the trigger.
	 */
	virtual void StartTrigger(AActor* TriggeredActor);

	/**
	 * @brief Called when the trigger is deactivated by an Actor.
	 * @param TriggeredActor The Actor that triggered the trigger.
	 */
	virtual void EndTrigger(AActor* TriggeredActor);

	UFUNCTION()
	virtual void OnTriggerComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnTriggerComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

protected:
	/**
	 * @brief The trigger box that is used to detect Actors.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers")
	TObjectPtr<UBoxComponent> TriggerBox;

	/**
	 * @brief The class of the Actor that can trigger the trigger.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers")
	TSubclassOf<AActor> TriggerClass = AActor::StaticClass();

	/**
	 * @brief If true, the trigger will need to be reset manually to be able to be triggered again.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers")
	bool bManualReset = false;
};
