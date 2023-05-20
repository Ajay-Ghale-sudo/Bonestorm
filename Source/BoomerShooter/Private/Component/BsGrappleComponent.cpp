// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomerShooter/Public/Component/BsGrappleComponent.h"
#include "Components/SphereComponent.h"


// Sets default values for this component's properties
UBsGrappleComponent::UBsGrappleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	

	// ...
}


// Called when the game starts
void UBsGrappleComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
	OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
	
	
}

void UBsGrappleComponent::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	
}

void UBsGrappleComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
