// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "Decapitator.generated.h"

class ABsSeveredHeadBase;
// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UDecapitator : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Interface for Actors that can decapitate others.
 */
class BOOMERSHOOTER_API IDecapitator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * @brief Called when an actor is decapitated.
	 * @param DecapitatedHead The head that was decapitated.
	 */
	virtual void DecapitatedActor(ABsSeveredHeadBase* DecapitatedHead) = 0;
};