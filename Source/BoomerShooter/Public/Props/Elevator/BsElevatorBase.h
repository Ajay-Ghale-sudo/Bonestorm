// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "BsElevatorBase.generated.h"

class UCapsuleComponent;

UCLASS()
class BOOMERSHOOTER_API ABsElevatorBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsElevatorBase();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor) override;

	/**
	 * @brief Generic function that activates the elevator.
	 **/
	UFUNCTION(BlueprintCallable, meta = (AllowInEventReceiver = "true"))
	void Activated();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void MoveToLocation(float DeltaTime);
protected:
	
	/**
	 * @brief Static mesh of the elevator. Set in blueprint.
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ElevatorMesh;

	/**
	 * @brief Capsule component of the elevator
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;
	
	/**
	 * @brief End location of the elevators lerp destination
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* ElevatorEndComponent;
	
	/**
	 * @brief Float curve the Lerp follows. Set in blueprint.
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* ElevatorFloatCurve;
	
	/**
	 * @brief Start location for the elevator to lerp from
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector LerpStartLocation;

	/**
	 * @brief End location for the elevator to lerp to
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector LerpEndLocation;
	
	/**
	 * @brief Set if elevator is activated
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsActivated = false;

	/**
	 * @brief Set if the elevator is going up
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bGoingUp = true;
};
