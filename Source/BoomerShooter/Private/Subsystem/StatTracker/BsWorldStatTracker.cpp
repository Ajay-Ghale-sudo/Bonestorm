// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/StatTracker/BsWorldStatTracker.h"

#include "Kismet/GameplayStatics.h"
#include "Subsystem/StatTracker/BsGameStatTracker.h"

void UBsWorldStatTracker::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UBsWorldStatTracker::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld()))
	{
		if (UBsGameStatTracker* GST = GI->GetSubsystem<UBsGameStatTracker>())
		{
			GST->StartTrackingPlayer();
		}
	}
}