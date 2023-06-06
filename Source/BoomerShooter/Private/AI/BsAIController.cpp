// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BsAIController.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/BsHealthComponent.h"

void ABsAIController::SetCurrentState(const EBsAIState NewState)
{
	PreviousState = CurrentState;
	CurrentState = NewState;
	ProcessStateChange();
	OnStateChanged.Broadcast();
}

void ABsAIController::ProcessStateChange()
{
	if (UBlackboardComponent* CurrentBlackboard = GetBlackboardComponent())
	{
		CurrentBlackboard->SetValueAsEnum(BsBlackboardKeys::CurrentState, static_cast<uint8>(CurrentState));
		CurrentBlackboard->SetValueAsEnum(BsBlackboardKeys::PreviousState, static_cast<uint8>(PreviousState));
	}
	
	if (CurrentState == EBsAIState::EAIS_Dead && PreviousState != CurrentState)
	{
		OnDeath();
	}
}

void ABsAIController::ProcessState()
{
	if (Blackboard)
	{
		const AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(BsBlackboardKeys::Target));
		const APawn* CurrentPawn = GetPawn();
		if (!Target || !CurrentPawn) return;
		
		const float Distance = FVector::Dist(Target->GetActorLocation(), CurrentPawn->GetActorLocation());

		if (bCanFlee && Distance <= FleeDistanceThreshold)
		{
			SetCurrentState(EBsAIState::EAIS_Flee);
		}
		else if (Distance > ChaseDistanceThreshold)
		{
			SetCurrentState(EBsAIState::EAIS_Chase);
		}
	}
}

void ABsAIController::Die()
{
	SetCurrentState(EBsAIState::EAIS_Dead);
}

void ABsAIController::OnDeath()
{
	if (UBrainComponent* Brain = GetBrainComponent())
	{
		Brain->StopLogic("Dead");
	}
}

void ABsAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		if (UBsHealthComponent* HealthComponent = InPawn->FindComponentByClass<UBsHealthComponent>())
		{
			HealthComponent->OnDeath.AddDynamic(this, &ABsAIController::Die);
		}
	}

	if (Blackboard)
	{
		Blackboard->SetValueAsVector(BsBlackboardKeys::POI, InPawn->GetActorLocation());
	}
}

void ABsAIController::OnUnPossess()
{
	if (const APawn* CurrentPawn = GetPawn())
	{
		if (UBsHealthComponent* HealthComponent = CurrentPawn->FindComponentByClass<UBsHealthComponent>())
		{
			HealthComponent->OnDeath.RemoveDynamic(this, &ABsAIController::Die);
		}
	}
	
	Super::OnUnPossess();
}
