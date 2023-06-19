﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BsHealthComponent.generated.h"

class UBsBleedDamageType;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsHealthComponentEvent);
DECLARE_MULTICAST_DELEGATE_OneParam(FBsHealthComponentExplosionEvent, FVector LaunchVector);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBsHealthComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void Heal(float Healing);

public:
	UPROPERTY(BlueprintAssignable)
	FBsHealthComponentEvent OnDeath;

	UPROPERTY(BlueprintAssignable)
	FBsHealthComponentEvent OnTookDamage;

	UPROPERTY(BlueprintAssignable)
	FBsHealthComponentEvent OnHealthChanged;

	FBsHealthComponentExplosionEvent OnExplosionHit;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ApplyDamage(float Damage);
	
	UFUNCTION()
	void ProcessDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
	void ProcessBleedDamage();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	TSet<TSubclassOf<UDamageType>> DamageTypesToIgnore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	TSoftObjectPtr<AActor> LastDamagedBy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY()
	const UBsBleedDamageType* CurrentBleedDamageType;
	
	float CurrentBleedDuration = 0.f;
	
	FTimerHandle BleedTimerHandle;
public:

	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE AActor* GetLastDamagedBy() const { return LastDamagedBy.Get(); }
};
