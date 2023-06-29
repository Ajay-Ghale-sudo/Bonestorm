// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BsEventTrigger.generated.h"

class UBoxComponent;
UCLASS()
class BOOMERSHOOTER_API ABsEventTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABsEventTrigger();

	UFUNCTION(BlueprintNativeEvent)
	void Triggered();

	UFUNCTION()
	void OnTriggerComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers")
	TSubclassOf<AActor> TriggerClass = AActor::StaticClass();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
