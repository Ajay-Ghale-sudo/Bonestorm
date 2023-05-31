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
	MoveToLocation(DeltaTime);
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
}

void ABsElevatorBase::MoveToLocation(float DeltaTime)
{
	if (!bIsActivated && ElevatorFloatCurve) return;
	UWorld* World = GetWorld();
	//ElevatorFloatCurve->FloatCurve.GetLastKey();
	// Elevator is going upwards
	LerpEndLocation = ElevatorEndComponent->GetComponentLocation();
	//FMath::Lerp(LerpStartLocation, LerpEndLocation
}


