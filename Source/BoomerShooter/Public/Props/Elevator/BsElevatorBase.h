// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "BsElevatorBase.generated.h"

class UCapsuleComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsElevatorEvent);

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

	UFUNCTION(BlueprintCallable)
	void SetActivated(bool bActivated);


public:
	/**
	 * @brief Event that is called when the elevator is activated
	 */
	UPROPERTY(BlueprintAssignable, Category = "Elevator|Event")
	FBsElevatorEvent OnElevatorActivated;

	/**
	 * @brief Event that is called when the elevator is deactivated
	 */
	UPROPERTY(BlueprintAssignable, Category = "Elevator|Event")
	FBsElevatorEvent OnElevatorDeactivated;
	
protected:
	virtual void BeginPlay() override;

	/**
	 * @brief Moves the elevator based on the float curve, the start and end location, and the current time
	 * @param DeltaTime Delta time between frames
	 */
	void MoveTick(float DeltaTime);
	
protected:
	
	/**
	 * @brief Static mesh of the elevator.
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
	UPROPERTY(BlueprintReadOnly, Category = "Elevator|Location")
	FVector LerpStartLocation;

	/**
	 * @brief End location for the elevator to lerp to
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "Elevator|Location")
	FVector LerpEndLocation;

	/**
	 * @brief Current time for the elevator movement
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "Elevator|Movement")
	float CurrentTime = 0.f;

	/**
	 * @brief Speed scale for the elevator movement.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Elevator|Movement", meta = (ClampMin = "0.0", ClampMax = "10.0", UIMin = "0.0", UIMax = "10.0"))
	float SpeedScale = 1.f;
	
	/**
	 * @brief Set if elevator is activated
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Elevator|State")
	bool bIsActivated = false;

	/**
	 * @brief Set if the elevator is going up
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator|State")
	bool bGoingUp = true;
};