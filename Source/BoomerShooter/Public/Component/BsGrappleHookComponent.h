// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BsGrappleHookComponent.generated.h"


class UNiagaraComponent;
class UNiagaraSystem;
class ABsGrappleProjectile;
class ACharacter;
class USceneComponent;

USTRUCT(BlueprintType)
struct FGrappleHookProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GrappleHook")
	TSubclassOf<ABsGrappleProjectile> GrappleProjectileClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="GrappleHook")
	ABsGrappleProjectile* GrappleProjectile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GrappleHook")
	float MaxDistance = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GrappleHook")
	float DistanceCompletionThreshold = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GrappleHook")
	float PullForce = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GrappleHook")
	float MaxAttachTime = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GrappleHook")
	float AttachTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GrappleHook")
	bool bIsAttached = false;

	FTimerHandle GrapplePullTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GrappleHook")
	UNiagaraSystem* GrappleFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GrappleHook")
	UNiagaraComponent* GrappleFXComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GrappleHook")
	USceneComponent* GrappleFXComponentAttachPoint;
	
	FTimerHandle GrappleFXUpdateTimerHandle;
	
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsGrappleHookComponentEvent);
DECLARE_MULTICAST_DELEGATE_OneParam(FBsGrappleHookComponentMoveEvent, FVector Movement);

/*
 * @brief A grapple hook can attach to a grapple point and then pull the owner towards it.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsGrappleHookComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBsGrappleHookComponent();

	ABsGrappleProjectile* FireGrappleHook(const FVector& StartLocation, const FVector& Direction);
	void DetachGrappleHook();
	void PullOwnerToGrapplePoint();
	void SetEffectedCharacter(ACharacter* Character);
	void SetGrappleFXAttachPoint(USceneComponent* AttachPoint);

public:
	UPROPERTY(BlueprintAssignable, Category="GrappleHook")
	FBsGrappleHookComponentEvent OnGrappleHookAttached;

	UPROPERTY(BlueprintAssignable, Category="GrappleHook")
	FBsGrappleHookComponentEvent OnGrappleHookDetached;

	FBsGrappleHookComponentMoveEvent OnGrappleHookPull;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void GrappleHookAttached();

	UFUNCTION()
	void GrappleHookDetached();

	bool OwnerCanSeeGrapplePoint() const;
	bool OwnerCanReachGrapplePoint() const;

	void InitFX();
	void UpdateGrappleFX();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GrappleHook")
	FGrappleHookProperties GrappleHookProperties;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GrappleHook")
	ACharacter* EffectedCharacter = nullptr;

public:
	FORCEINLINE bool IsGrappleHookAttached() const { return GrappleHookProperties.bIsAttached; }
	FORCEINLINE bool IsGrappleHookProjectileValid() const { return GrappleHookProperties.GrappleProjectile != nullptr; }
};
