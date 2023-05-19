// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BsGrappleComponent.generated.h"

class USphereComponent;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

	
public:
	// Sets default values for this component's properties
	UBsGrappleComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/*
	 * Protected UFunctions
	 */

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	


protected:
	/*
	 * Protected UProperties
	 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grapple")
	USphereComponent* SphereComponent;


	
};
