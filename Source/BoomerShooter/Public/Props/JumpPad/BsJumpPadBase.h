// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BsJumpPadBase.generated.h"

class UArrowComponent;
class USphereComponent;
UCLASS()
class BOOMERSHOOTER_API ABsJumpPadBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsJumpPadBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                        int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpPad")
	float JumpForce = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpPad")
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpPad")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpPad")
	UArrowComponent* LaunchDirection;
	

public:

};
