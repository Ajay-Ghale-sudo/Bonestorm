// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BsDoorBase.h"
#include "Data/KeyType.h"
#include "BsLockedDoor.generated.h"

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API ABsLockedDoor : public ABsDoorBase
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* Interactor) override;
	
protected:
	/**
	 * @brief Changed the Locked State of the Door
	 * @param bIsLocked The New Locked State
	 */
	void LockDoor(bool bIsLocked);

protected:
	/**
	 * @brief The Type of Key that is required to open the Door
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	EBsKeyType RequiredKeyType = EBsKeyType::EKT_None;
};
