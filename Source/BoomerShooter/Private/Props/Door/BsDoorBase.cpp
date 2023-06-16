// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Door/BsDoorBase.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/BsDoorIndicatorWidget.h"


// Sets default values
ABsDoorBase::ABsDoorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	SetRootComponent(DoorMesh);
	DoorStatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DoorWidget"));
	DoorStatusWidgetComponent->SetupAttachment(RootComponent);
	DoorStatusWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
}

void ABsDoorBase::Interact(AActor* Interactor)
{
	if (bOpenedElsewhere) return;
	ToggleDoor();
}

// Called when the game starts or when spawned
void ABsDoorBase::BeginPlay()
{
	Super::BeginPlay();
	
	OnDoorStateChanged.AddDynamic(this, &ABsDoorBase::UpdateTargetTransform);

	InitialTransform = GetTransform();
	TargetTransform = GetTransform();

	if (DoorStatusWidgetComponent)
	{
		if (UBsDoorIndicatorWidget* DoorWidget = Cast<UBsDoorIndicatorWidget>(DoorStatusWidgetComponent->GetWidget()))
		{
			DoorWidget->SetDoorIndicator(this);
		}
	}
}

void ABsDoorBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DoorTick(DeltaSeconds);
}

void ABsDoorBase::DoorTick(float DeltaSeconds)
{
	if (!GetTransform().Equals(TargetTransform))
	{
		const FVector Location = GetActorTransform().GetLocation();
		const FRotator Rotation = GetActorTransform().GetRotation().Rotator();

		const FVector NewLocation = FMath::VInterpTo(Location, TargetTransform.GetLocation(), DeltaSeconds, DoorSpeed);
		const FRotator NewRotation = FMath::RInterpTo(Rotation, TargetTransform.GetRotation().Rotator(), DeltaSeconds, DoorSpeed);
		
		SetActorLocationAndRotation(NewLocation, NewRotation);
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void ABsDoorBase::UpdateTargetTransform()
{
	TargetTransform = InitialTransform + (bIsOpen ? OpenedTransform : ClosedTransform);
	SetActorTickEnabled(true);
}

void ABsDoorBase::ToggleDoor()
{	
	if (bIsOpen)
	{
		CloseDoor();
	}
	else
	{
		OpenDoor();
	}
}

void ABsDoorBase::OpenDoor()
{
	if (!CanOpen()) return;
	bIsOpen = true;
	OnDoorStateChanged.Broadcast();
}

void ABsDoorBase::CloseDoor()
{
	bIsOpen = false;
	OnDoorStateChanged.Broadcast();
}

void ABsDoorBase::SetLocked(bool bLock)
{
	bLocked = bLock;
	OnDoorStateChanged.Broadcast();
}

bool ABsDoorBase::CanOpen()
{
	return !bLocked;
}