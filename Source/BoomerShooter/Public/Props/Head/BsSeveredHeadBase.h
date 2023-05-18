// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BsSeveredHeadBase.generated.h"

UCLASS()
class BOOMERSHOOTER_API ABsSeveredHeadBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsSeveredHeadBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// TODO: This should be a Skeletal Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head")
	UStaticMeshComponent* HeadMesh;

public:
	FORCEINLINE UStaticMeshComponent* GetHeadMesh() const { return HeadMesh; }
};
