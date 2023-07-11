﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BsTeleportNode.generated.h"

class UArrowComponent;
class UBoxComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsTeleportNodeEvent);

UCLASS()
class BOOMERSHOOTER_API ABsTeleportNode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsTeleportNode();

	FTransform GetArrivalTransform() const; 
public:
	UPROPERTY(BlueprintAssignable, Category = "Teleport Node")
	FBsTeleportNodeEvent OnTeleported;

	UPROPERTY(BlueprintAssignable, Category = "Teleport Node")
	FBsTeleportNodeEvent OnTargetArrived;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnTeleportCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep,
	                                     const FHitResult& SweepResult);

	virtual void TeleportTarget(AActor* Target) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport Node")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport Node")
	UBoxComponent* TeleportCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport Node")
	ABsTeleportNode* TargetNode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport Node")
	UArrowComponent* ArrivalLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport Node")
	TSet<TSubclassOf<AActor>> SupportedTypes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport Node")
	bool bActive = true;
};