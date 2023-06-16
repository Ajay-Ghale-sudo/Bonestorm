// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "BsDoorBase.generated.h"

class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsDoorEvent);

UCLASS()
class BOOMERSHOOTER_API ABsDoorBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsDoorBase();

	virtual void Interact(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable, meta = (AllowInEventReceiver = "true"))
	virtual void OpenDoor();

	UFUNCTION(BlueprintCallable, meta = (AllowInEventReceiver = "true"))
	virtual void CloseDoor();

	UFUNCTION(BlueprintCallable)
	virtual void SetLocked(bool bLock);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBsDoorEvent OnDoorStateChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	virtual void DoorTick(float DeltaSeconds);

	/**
	 * @brief Updates the target transform based on the current state of the door and enables Tick.
	 */
	UFUNCTION()
	void UpdateTargetTransform();
	
	UFUNCTION(BlueprintCallable, meta = (AllowInEventReceiver = "true"))
	void ToggleDoor();
	
	virtual bool CanOpen();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	UWidgetComponent* DoorStatusWidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsOpen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bOpenedElsewhere = false;

	/**
	 * @brief The relative transform to apply to InitialTransform when Closed.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	FTransform ClosedTransform;

	/**
	 * @brief The relative transform to apply to InitialTransform when Opened.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	FTransform OpenedTransform;

	/**
	 * @brief The initial transform of the door. Set in BeginPlay.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	FTransform InitialTransform;

	/**
	 * @brief The target transform of the door. Set in UpdateTargetTransform.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	FTransform TargetTransform;

	/**
	 * @brief The speed at which the door opens and closes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	float DoorSpeed = 5.f;

public:
	FORCEINLINE bool IsLocked() const { return bLocked; }
};
