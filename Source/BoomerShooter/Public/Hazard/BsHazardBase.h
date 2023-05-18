// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BsHazardBase.generated.h"

class IReceiveDamage;
class UBoxComponent;

UCLASS()
class BOOMERSHOOTER_API ABsHazardBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsHazardBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	virtual void DamageTick();
	
	UFUNCTION()
	void OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep,
	                                const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hazard")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hazard")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard")
	TArray<IReceiveDamage*> OverlappingActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hazard")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hazard")
	float DamageInterval = 1.f;

	FTimerHandle DamageTimerHandle;
	
public:

};