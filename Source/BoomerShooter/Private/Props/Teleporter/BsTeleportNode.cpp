// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Teleporter/BsTeleportNode.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ABsTeleportNode::ABsTeleportNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	TeleportCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TeleportCollision"));
	TeleportCollision->SetupAttachment(RootComponent);

	ArrivalLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrivalLocation"));
	ArrivalLocation->SetupAttachment(RootComponent);
	ArrivalLocation->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
}

// Called when the game starts or when spawned
void ABsTeleportNode::BeginPlay()
{
	Super::BeginPlay();

	if (TeleportCollision)
	{
		TeleportCollision->OnComponentBeginOverlap.AddDynamic(this, &ABsTeleportNode::OnTeleportCollisionBeginOverlap);
	}
}

FTransform ABsTeleportNode::GetArrivalTransform() const
{
	FTransform Transform = GetActorTransform();
	if (ArrivalLocation)
	{
		Transform = ArrivalLocation->GetComponentTransform();
	}

	return Transform;
}

void ABsTeleportNode::OnTeleportCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	TeleportTarget(OtherActor);
}

void ABsTeleportNode::TeleportTarget(AActor* Target) const
{
	if (!bActive || !Target || !TargetNode) return;
	
	// @NOTE: This is a very simple implementation, but it works for our purposes. This does not support inheritance.
	if (SupportedTypes.Contains(Target->GetClass()))
	{
		const FTransform Destination = TargetNode->GetArrivalTransform();
		Target->TeleportTo(Destination.GetLocation(), Target->GetActorRotation(), false, false);
		OnTeleported.Broadcast();
		TargetNode->OnTargetArrived.Broadcast();
	}
}