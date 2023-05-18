// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Props/Key/BsKeyBase.h"
#include "BsInventoryComponent.generated.h"


class ABsKeyBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBsInventoryComponent();


	void AddKey(ABsKeyBase* Key);
	bool HasKey(EBsKeyType BsKey);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FBsKeyData> Keys;
	
public:

};
