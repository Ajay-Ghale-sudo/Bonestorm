// Fill out your copyright notice in the Description page of Project Settings.

#include "Props/Elevator/BsElevatorBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABsElevatorBase::ABsElevatorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComponent);
	
	ElevatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorMesh"));
	ElevatorMesh->SetupAttachment(CapsuleComponent);
	
	ElevatorEndComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ElevatorEnd"));
	ElevatorEndComponent->SetupAttachment(CapsuleComponent);
}

// Called every frame
void ABsElevatorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveTick(DeltaTime);
}

void ABsElevatorBase::Interact(AActor* Interactor)
{
	
}

// Called when the game starts or when spawned
void ABsElevatorBase::BeginPlay()
{
	Super::BeginPlay();
	LerpStartLocation = ElevatorMesh->GetRelativeLocation();
	LerpEndLocation = ElevatorEndComponent->GetRelativeLocation();
}

void ABsElevatorBase::Activated()
{
	bIsActivated = true;
	SetActorTickEnabled(true);
}

void ABsElevatorBase::SetActivated(bool bActivated)
{
	bIsActivated = bActivated;

	if (bActivated) OnElevatorActivated.Broadcast();
	else OnElevatorDeactivated.Broadcast();

	SetActorTickEnabled(bIsActivated);
}

void ABsElevatorBase::MoveTick(float DeltaTime)
{
	if (!bIsActivated || !ElevatorFloatCurve) return;
	
	// Swizzles between going up and going down based on positive or negative input
	float CurveEnd = ElevatorFloatCurve->FloatCurve.GetLastKey().Value;
	CurrentTime = FMath::Clamp(CurrentTime + DeltaTime * (bGoingUp ? 1.f : -1.f), 0.f, CurveEnd);
	
	if (CurrentTime < CurveEnd && CurrentTime > 0.f)
	{
		FVector CurrentLocation = FMath::Lerp(LerpStartLocation, LerpEndLocation, CurrentTime);
		ElevatorMesh->SetRelativeLocation(CurrentLocation);
	}
	else
	{
		OnElevatorDestinationReached.Broadcast();
	}
}