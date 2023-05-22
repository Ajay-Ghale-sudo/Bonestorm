// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "BsDoorBase.generated.h"


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

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBsDoorEvent OnDoorStateChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, meta = (AllowInEventReceiver = "true"))
	void ToggleDoor();

	
	virtual bool CanOpen();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	UStaticMeshComponent* DoorMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsOpen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bOpenedElsewhere = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float RotationAmount = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	FRotator StartingRotation;
	
};
