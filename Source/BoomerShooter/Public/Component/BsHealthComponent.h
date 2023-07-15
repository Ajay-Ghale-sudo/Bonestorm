// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BsHealthComponent.generated.h"

class UBsBleedDamageType;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsHealthComponentEvent);
DECLARE_MULTICAST_DELEGATE_OneParam(FBsHealthComponentExplosionEvent, FVector LaunchVector);
DECLARE_MULTICAST_DELEGATE_OneParam(FBsHealthComponentDamagedByEvent, AActor* DamagedBy);
DECLARE_MULTICAST_DELEGATE_OneParam(FBsHealthComponentHealthChangedEvent, float Damage);

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
	FBsHealthComponentEvent OnDecapitated;

	UPROPERTY(BlueprintAssignable)
	FBsHealthComponentEvent OnTookDamageEvent;
	FBsHealthComponentHealthChangedEvent OnTookDamage;
	FBsHealthComponentHealthChangedEvent OnHealedDamage;

	UPROPERTY(BlueprintAssignable)
	FBsHealthComponentEvent OnHealthChanged;

	UPROPERTY()
	FBsHealthComponentEvent OnLowHealth;

	FBsHealthComponentExplosionEvent OnExplosionHit;
	FBsHealthComponentDamagedByEvent OnDamagedBy;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ApplyDamage(float Damage, const UDamageType* DamageType = nullptr);
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DamageModifier = 1.f;

	UPROPERTY()
	const UBsBleedDamageType* CurrentBleedDamageType;
	
	float CurrentBleedDuration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0") )
	float LowHealthThreshold = 0.25f;
	
	FTimerHandle BleedTimerHandle;
public:

	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE AActor* GetLastDamagedBy() const { return LastDamagedBy.Get(); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthPercentage() const { return (MaxHealth != 0.f ? CurrentHealth / MaxHealth : 0.f); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsLowHealth() const { return  GetHealthPercentage() < LowHealthThreshold; }
};
