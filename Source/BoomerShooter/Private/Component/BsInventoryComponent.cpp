// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BsInventoryComponent.h"

#include "Props/Head/BsSeveredHeadBase.h"


// Sets default values for this component's properties
UBsInventoryComponent::UBsInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UBsInventoryComponent::AddKey(ABsKeyBase* Key)
{
	if (Key)
	{
		FBsKeyData NewKey;
		NewKey.CopyKeyData(Key->GetKeyData());
		Keys.Add(NewKey);
		OnKeyAdded.Broadcast(NewKey);
		Key->Destroy();
	}
}

bool UBsInventoryComponent::HasKey(EBsKeyType BsKey)
{
	for (const FBsKeyData Key : Keys)
	{
		if (Key.KeyType == BsKey)
		{
			return true;
		}
	}
	return false;
}

void UBsInventoryComponent::AddSeveredHead(ABsSeveredHeadBase* SeveredHead)
{
	if (SeveredHead && SeveredHeads.Num() < MaxSeveredHeads && !SeveredHeads.Contains(SeveredHead))
	{
		SeveredHeads.AddUnique(SeveredHead);
		OnSeveredHeadAdded.Broadcast(SeveredHead);
	}
}

ABsSeveredHeadBase* UBsInventoryComponent::GetNextHead()
{
	if (!SeveredHeads.IsEmpty())
	{
		return SeveredHeads.Pop();
	}

	return nullptr;
}

// Called when the game starts
void UBsInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

