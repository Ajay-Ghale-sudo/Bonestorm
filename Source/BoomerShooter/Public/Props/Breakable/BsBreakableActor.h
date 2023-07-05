// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BsBreakableActor.generated.h"

class UBsHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsBreakableActorEvent);

UCLASS()
class BOOMERSHOOTER_API ABsBreakableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsBreakableActor();

public:
	UPROPERTY(BlueprintAssignable)
	FBsBreakableActorEvent OnBreak;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Break();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Breakable")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Breakable")
	UBsHealthComponent* HealthComponent;
};