// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BsSeveredHeadBase.generated.h"

class ABsProjectileBase;
class UDamageType;
class UWidgetComponent;

DECLARE_MULTICAST_DELEGATE(FBsSeveredHeadEvent)
UCLASS()
class BOOMERSHOOTER_API ABsSeveredHeadBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsSeveredHeadBase();

	/**
	 * @brief Sets the bIsAttached bool.
	 * @param bAttached If the SeveredHead is attached or not.
	 */
	void SetAttached(bool bAttached);
	
	ABsProjectileBase* CreateProjectile(TSubclassOf<ABsProjectileBase> ProjectileClass, const FTransform &SpawnTransform, FActorSpawnParameters& SpawnParameters);

	UFUNCTION()
	float ParryDamage(float Damage);

	UFUNCTION()
	float BlockDamage(float Damage);

	float Consume();
public:
	
	FBsSeveredHeadEvent OnDetachedHead;
	FBsSeveredHeadEvent OnHeadChargeChanged;

	bool SetAttachable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief Enables Collision and binds the OnMeshOverlapBegin delegate.
	 */
	void EnableMeshOverlap();

	/**
	 * @brief Disables Collision and clears the OnMeshOverlapBegin delegate.
	 */
	void DisableMeshOverlap();
	
	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);

protected:
	// TODO: This should be a Skeletal Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	UStaticMeshComponent* HeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head")
	bool bIsAttached;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head")
	bool bAttachable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	TSubclassOf<UDamageType> HeadDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Head")
	UWidgetComponent* HeadWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Head")
	float HealingAmount = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	float ChargeCost = 10.f;

	float CurrentCharge = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	float MaxCharge = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	float ParryingChargeMultiplier = 1.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	float BlockChargeMultiplier = 1.25f;

public:
	FORCEINLINE UStaticMeshComponent* GetHeadMesh() const { return HeadMesh; }
	FORCEINLINE float GetCurrentCharge() const { return CurrentCharge; }
	FORCEINLINE float GetHeadHealAmount() const { return HealingAmount; }
	FORCEINLINE bool GetHeadAttachable() const { return bAttachable; }
};
