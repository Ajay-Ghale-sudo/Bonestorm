// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomerShooter/Public/Component/BsGrappleComponent.h"

#include "Components/SphereComponent.h"


// Sets default values for this component's properties
UBsGrappleComponent::UBsGrappleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("GrappleCollision"));
	

	// ...
}


// Called when the game starts
void UBsGrappleComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UBsGrappleComponent::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped with grapple sphere component"));
}

void UBsGrappleComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped overlapping with grapple sphere component"));
}
