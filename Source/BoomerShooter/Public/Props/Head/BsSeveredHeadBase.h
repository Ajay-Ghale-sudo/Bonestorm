// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BsSeveredHeadBase.generated.h"

class ABsProjectileBase;

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

public:
	FORCEINLINE UStaticMeshComponent* GetHeadMesh() const { return HeadMesh; }
};
