// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Props/Key/BsKeyBase.h"
#include "BsInventoryComponent.generated.h"


class ABsSeveredHeadBase;
class ABsKeyBase;


DECLARE_MULTICAST_DELEGATE_OneParam(FBsInventoryComponentKeyAdded, EBsKeyType);
DECLARE_MULTICAST_DELEGATE_OneParam(FBsInventoryComponentSeveredHeadAdded, ABsSeveredHeadBase*);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBsInventoryComponent();


	void AddKey(ABsKeyBase* Key);
	UFUNCTION(BlueprintPure)
	bool HasKey(EBsKeyType BsKey);

	void AddSeveredHead(ABsSeveredHeadBase* SeveredHead);
	ABsSeveredHeadBase* GetNextHead();

public:
	FBsInventoryComponentKeyAdded OnKeyAdded;
	FBsInventoryComponentSeveredHeadAdded OnSeveredHeadAdded;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FBsKeyData> Keys;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<ABsSeveredHeadBase*> SeveredHeads;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSeveredHeads = 5;
	
public:

};
