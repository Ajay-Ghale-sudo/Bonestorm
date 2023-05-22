// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BsGrapplePoint.generated.h"

class UBsGrapplePointComponent;

UCLASS()
class BOOMERSHOOTER_API ABsGrapplePoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsGrapplePoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Grapple")
	UBsGrapplePointComponent* GrappleArea;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Grapple")
	UStaticMeshComponent* Mesh;

};
