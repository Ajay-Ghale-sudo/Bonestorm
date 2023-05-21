// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BsEventReceiverComponent.h"


// Sets default values for this component's properties
UBsEventReceiverComponent::UBsEventReceiverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBsEventReceiverComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	
}

void UBsEventReceiverComponent::ReceiveEvent()
{
	EventReceived.Broadcast();

	if (AActor* CurrentOwner = GetOwner())
	{
		if (UFunction* CallbackFunction = CurrentOwner->FindFunction(CallbackFunctionName))
		{
			CurrentOwner->ProcessEvent(CallbackFunction, nullptr);
		}
	}
}

TArray<FString> UBsEventReceiverComponent::GetFunctionOptions() const
{
	TArray<FString> Options;
#if WITH_EDITORONLY_DATA
	if (const AActor* CurrentOwner = GetOwner())
	{
		if (const UClass* OwnerClass = CurrentOwner->GetClass())
		{
			for (TFieldIterator<UFunction> FuncIter(OwnerClass); FuncIter; ++FuncIter)
			{
				const UFunction* Function = *FuncIter;
				if (Function->GetMetaData(TEXT("AllowInEventReceiver")) == TEXT("true"))
				{
					Options.Add(Function->GetName());
				}
			}
		}
	}
#endif
	return Options;
}

