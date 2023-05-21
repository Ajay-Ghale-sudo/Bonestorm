// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BsEventEmitterComponent.h"

#include "Component/BsEventReceiverComponent.h"
#include "Interfaces/EventReceiver.h"


// Sets default values for this component's properties
UBsEventEmitterComponent::UBsEventEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UBsEventEmitterComponent::EmitEvent()
{
	Event.Broadcast();
}

void UBsEventEmitterComponent::AddEventReceiver(AActor* EventReceiver)
{
	if (IEventReceiver* IReceiver = Cast<IEventReceiver>(EventReceiver))
	{
		BindToEventReceiver(IReceiver);
		EventReceivers.Add(EventReceiver);
	}
}

void UBsEventEmitterComponent::AddEventReceiver(UBsEventReceiverComponent* EventReceiver)
{
	if (EventReceiver)
	{
		BindToEventReceiver(EventReceiver);
		if (AActor* ReceiverOwner = EventReceiver->GetOwner())
		{
			EventReceivers.Add(ReceiverOwner);
		}
	}
}

void UBsEventEmitterComponent::RemoveEventReceiver(UBsEventReceiverComponent* EventReceiver)
{
	Event.RemoveAll(EventReceiver);
}

void UBsEventEmitterComponent::RemoveEventReceiver(AActor* EventReceiver)
{
	if (EventReceiver)
	{
		EventReceivers.Remove(EventReceiver);
	}
}

// Called when the game starts
void UBsEventEmitterComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (const auto EventReceiver : EventReceivers)
	{
		if (IEventReceiver* IReceiver = Cast<IEventReceiver>(EventReceiver))
		{
			BindToEventReceiver(IReceiver);
		}
		else if (UBsEventReceiverComponent* ReceiverComponent = EventReceiver->GetComponentByClass<UBsEventReceiverComponent>())
		{
			BindToEventReceiver(ReceiverComponent);
		}
	}
}

void UBsEventEmitterComponent::BindToEventReceiver(IEventReceiver* EventReceiver)
{
	Event.AddDynamic(EventReceiver, &IEventReceiver::ReceiveEvent);
}

void UBsEventEmitterComponent::BindToEventReceiver(UBsEventReceiverComponent* EventReceiver)
{
	Event.AddDynamic(EventReceiver, &UBsEventReceiverComponent::ReceiveEvent);
}

void UBsEventEmitterComponent::UnBindEventReceiver(IEventReceiver* EventReceiver)
{
	if (const UObject* EventReceiverObject = Cast<UObject>(EventReceiver))
	{
		Event.RemoveAll(EventReceiverObject);
	}
}


void UBsEventEmitterComponent::UnBindEventReceiver(UBsEventReceiverComponent* EventReceiver)
{
	Event.RemoveAll(EventReceiver);
}
