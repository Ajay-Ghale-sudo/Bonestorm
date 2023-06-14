// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/AI/BsAISubsystem.h"
#include "AI/BsAIDirector.h"

void UBsAISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	InitAIDirector();
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UBsAISubsystem::OnPostWorldInitialization);
}

void UBsAISubsystem::InitAIDirector()
{
	AIDirector = NewObject<UBsAIDirector>(this, TEXT("AIDirector"));
	if (AIDirector)
	{
		AIDirector->Init();
	}
}

void UBsAISubsystem::OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS)
{
	InitAIDirector();
}