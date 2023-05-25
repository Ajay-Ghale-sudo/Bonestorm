// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BsEventReceiverComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsEventReceiverComponentEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsEventReceiverComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class UBsEventEmitterComponent;
	
public:
	// Sets default values for this component's properties
	UBsEventReceiverComponent();

public:
	UPROPERTY(BlueprintAssignable, Category = "EventReceiver")
	FBsEventReceiverComponentEvent EventReceived;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(meta =  (AllowInEventReceiver = "true"))
	void ReceiveEvent();

	UFUNCTION(CallInEditor)
	TArray<FString> GetFunctionOptions() const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EventReceiver", meta = (GetOptions = "GetFunctionOptions"))
	FName CallbackFunctionName;
};
