// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/GrapplePoint/BsGrapplePoint.h"

#include "Component/BsGrapplePointComponent.h"


// Sets default values
ABsGrapplePoint::ABsGrapplePoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrapplePoint"));
	SetRootComponent(Mesh);
	GrappleArea = CreateDefaultSubobject<UBsGrapplePointComponent>(TEXT("GrappleComponent"));
	GrappleArea->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ABsGrapplePoint::BeginPlay()
{
	Super::BeginPlay();
	
}

