// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "BsAIDirector.generated.h"

class ABsCharacter;
class ABsEnemyBase;
class ABsArena;

/**
 * 
 */
UCLASS()
class BOOMERSHOOTER_API UBsAIDirector : public UObject
{
	GENERATED_BODY()

public:

	void Init();
protected:
	void InitArenas();

	/**
	 * @brief Finds all enemies in the world and adds them to the active enemies set.
	 */
	void InitEnemies();

	/**
	 * @brief Makes an Enemy focus the player.
	 * @param Enemy Enemy to make focus the player.
	 */
	void MakeEnemyFocusPlayer(ABsEnemyBase* Enemy);

	UFUNCTION()
	void OnArenaStarted(ABsArena* StartedArena);
	UFUNCTION()
	void OnArenaFinished(ABsArena* StartedArena);

	UFUNCTION()
	void OnActorSpawned(AActor* SpawnedActor);

	UFUNCTION()
	void OnArenaEnemySpawned(ABsEnemyBase* Enemy);

	UFUNCTION()
	void OnEnemyDeath(ABsEnemyBase* Enemy);
	
protected:
	FDelegateHandle OnActorSpawnedDelegateHandle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<ABsArena> ActiveArena;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TArray<ABsArena*> Arenas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TSet<ABsEnemyBase*> ActiveEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	ABsCharacter* Player;
};