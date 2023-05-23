// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Head/BsSeveredHeadBase.h"

#include "Component/BsInventoryComponent.h"


// Sets default values
ABsSeveredHeadBase::ABsSeveredHeadBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetSimulatePhysics(true);
	SetRootComponent(HeadMesh);
}

// Called when the game starts or when spawned
void ABsSeveredHeadBase::BeginPlay()
{
	Super::BeginPlay();

	if (HeadMesh)
	{
		HeadMesh->OnComponentBeginOverlap.AddDynamic(this, &ABsSeveredHeadBase::OnMeshOverlapBegin);
	}
	
}


void ABsSeveredHeadBase::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (UBsInventoryComponent* Inventory = OtherActor->FindComponentByClass<UBsInventoryComponent>())
	{
		Inventory->AddSeveredHead(this);
	}
}


