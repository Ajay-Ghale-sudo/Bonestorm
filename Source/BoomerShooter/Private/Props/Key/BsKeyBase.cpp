// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Key/BsKeyBase.h"

#include "Component/BsInventoryComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
ABsKeyBase::ABsKeyBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	KeyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("KeyMesh"));
	SetRootComponent(KeyMesh);

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	PickupSphere->SetupAttachment(KeyMesh);
}

// Called when the game starts or when spawned
void ABsKeyBase::BeginPlay()
{
	Super::BeginPlay();

	if (PickupSphere)
	{
		PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &ABsKeyBase::OnSphereOverlapBegin);
		PickupSphere->OnComponentEndOverlap.AddDynamic(this, &ABsKeyBase::OnSphereOverlapEnd);
	}
	
}

void ABsKeyBase::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UBsInventoryComponent* Inventory = OtherActor->FindComponentByClass<UBsInventoryComponent>())
	{
		Inventory->AddKey(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No inventory component found on %s"), *OtherActor->GetName());
	}
}

void ABsKeyBase::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

