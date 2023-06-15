// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Data/BsArenaConfig.h"
#include "GameFramework/Actor.h"
#include "BsArena.generated.h"

class UBoxComponent;
class ABsDoorBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBsArenaEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBsArenaEventWithCaller, ABsArena*, Arena);

UCLASS()
class BOOMERSHOOTER_API ABsArena : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsArena();

public:
	FBsArenaEvent OnArenaStarted;
	FBsArenaEvent OnArenaFinished;

	FBsArenaEventWithCaller OnThisArenaStarted;
	FBsArenaEventWithCaller OnThisArenaFinished;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnArenaVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void StartArena();
	
	UFUNCTION()
	void EndArena();

	UFUNCTION()
	void ActivateSpawner();

	UFUNCTION()
	void OnSpawnedEnemyDeath();

	UFUNCTION()
	void NextWave();
	
	UFUNCTION()
	void SetDoorsLocked(bool bLockDoors);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena|Doors")
	TArray<ABsDoorBase*> LockableDoors;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena")
	UBoxComponent* ArenaVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena")
	bool bArenaActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena|Enemies")
	FBsArenaConfig ArenaConfig;

public:
	FORCEINLINE FBsArenaConfig GetArenaConfig() const { return ArenaConfig; }
};