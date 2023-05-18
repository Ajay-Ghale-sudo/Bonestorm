// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Data/KeyData.h"
#include "GameFramework/Actor.h"
#include "BsKeyBase.generated.h"

class USphereComponent;

UCLASS()
class BOOMERSHOOTER_API ABsKeyBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsKeyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	FBsKeyData KeyData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	USkeletalMeshComponent* KeyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	USphereComponent* PickupSphere;
	
public:
FORCEINLINE FBsKeyData GetKeyData() const { return KeyData; }
};
