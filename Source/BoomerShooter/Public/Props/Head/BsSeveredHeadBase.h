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

	UFUNCTION()
	/**
	 * @brief Shows the WidgetComponent.
	 */
	void ShowWidget() const;

	UFUNCTION()
	/**
	 * @brief Hides the WidgetComponent.
	 */
	void HideWidget() const;
	
	/**
	 * @brief Attaches this to the Component.
	 * @param InParent Parent to attach to.
	 * @param SocketName Socket on the parent to attach to.
	 */
	void AttachHeadToComponent(USceneComponent* InParent, const FName& SocketName);

	/**
	 * @brief Detach the head.
	 */
	void DetachHead();
	
	ABsProjectileBase* CreateProjectile(TSubclassOf<ABsProjectileBase> ProjectileClass, const FTransform &SpawnTransform, FActorSpawnParameters& SpawnParameters);

	UFUNCTION()
	float ParryDamage(float Damage);

	UFUNCTION()
	float BlockDamage(float Damage);

	float Consume();

public:
	
	FBsSeveredHeadEvent OnDetachedHead;
	FBsSeveredHeadEvent OnHeadChargeChanged;

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

	/**
	 * @brief Applies an impulse to head to make it fly off.
	 */
	void PopHead() const;

	/**
	 * @brief Depletes the Charge by the ChargeCost.
	 */
	void DepleteCharge();
	
	/**
	 * @brief Depletes the Charge by the given Amount.
	 * @param Amount Amount to Deplete the Charge by.
	 */
	void DepleteCharge(const float Amount);

protected:
	// TODO: This should be a Skeletal Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	UStaticMeshComponent* HeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head")
	bool bIsAttached = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	TSubclassOf<UDamageType> HeadDamageType = UDamageType::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Head")
	TSubclassOf<ABsProjectileBase> HeadProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Head")
	UWidgetComponent* HeadWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Head")
	float HealingAmount = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	float ChargeCost = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head")
	float CurrentCharge = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	float MaxCharge = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	float ParryingChargeMultiplier = 1.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	float BlockChargeMultiplier = 1.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Head")
	float BlockDamageReductionMultiplier = 0.40f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	FVector SeveredHeadScale = FVector(0.4f, 0.4f, 0.4f);

public:
	FORCEINLINE UStaticMeshComponent* GetHeadMesh() const { return HeadMesh; }
	FORCEINLINE float GetCurrentCharge() const { return CurrentCharge; }
	FORCEINLINE float GetHeadHealAmount() const { return HealingAmount; }
	FORCEINLINE bool CanBeAttached() const { return !bIsAttached && CurrentCharge > 0.f; }
};
