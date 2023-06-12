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
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers")
	UBoxComponent* TriggerBox;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
