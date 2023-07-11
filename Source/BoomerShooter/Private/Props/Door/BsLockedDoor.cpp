// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Door/BsLockedDoor.h"
#include "Component/BsInventoryComponent.h"

void ABsLockedDoor::Interact(AActor* Interactor)
{
	if (UBsInventoryComponent* Inventory = Interactor->FindComponentByClass<UBsInventoryComponent>())
	{
		if (Inventory->HasKey(RequiredKeyType))
		{
			LockDoor(false);
			Super::Interact(Interactor);
		}
	}
}

void ABsLockedDoor::LockDoor(bool bIsLocked)
{
	bLocked = bIsLocked;
}
