// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BsEventEmitterComponent.generated.h"


class UBsEventReceiverComponent;
class IEventReceiver;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsEventEmitterComponentEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsEventEmitterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBsEventEmitterComponent();

	UFUNCTION(BlueprintCallable)
	void EmitEvent();
	
	void AddEventReceiver(AActor* EventReceiver);
	void AddEventReceiver(UBsEventReceiverComponent* EventReceiver);
	
	void RemoveEventReceiver(AActor* EventReceiver);
	void RemoveEventReceiver(UBsEventReceiverComponent* EventReceiver);


public:
	UPROPERTY(BlueprintAssignable, Category = "EventEmitter")
	FBsEventEmitterComponentEvent Event;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void BindToEventReceiver(IEventReceiver* EventReceiver);
	void BindToEventReceiver(UBsEventReceiverComponent* EventReceiver);
	
	void UnBindEventReceiver(IEventReceiver* EventReceiver);
	void UnBindEventReceiver(UBsEventReceiverComponent* EventReceiver);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventEmitter")
	TArray<AActor*> EventReceivers;
	
public:

};
