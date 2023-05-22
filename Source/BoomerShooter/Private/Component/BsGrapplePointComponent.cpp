// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomerShooter/Public/Component/BsGrapplePointComponent.h"


// Sets default values for this component's properties
UBsGrapplePointComponent::UBsGrapplePointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	

	// ...
}


// Called when the game starts
void UBsGrapplePointComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
	OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
}

void UBsGrapplePointComponent::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	
}

void UBsGrapplePointComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
