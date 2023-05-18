// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazard/BsHazardBase.h"

#include "Components/BoxComponent.h"
#include "Interfaces/ReceiveDamage.h"


// Sets default values
ABsHazardBase::ABsHazardBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ABsHazardBase::BeginPlay()
{
	Super::BeginPlay();

	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABsHazardBase::OnBoxComponentBeginOverlap);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABsHazardBase::OnBoxComponentEndOverlap);
	}
	
}

void ABsHazardBase::DamageTick()
{
	for (auto DamageReceiver : OverlappingActors)
	{
		DamageReceiver->ReceiveHazardDamage(this, Damage);
	}
}


void ABsHazardBase::OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (IReceiveDamage* DamageReceiver = Cast<IReceiveDamage>(OtherActor))
	{
		OverlappingActors.AddUnique(DamageReceiver);
		if (!DamageTimerHandle.IsValid())
		{
			GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ABsHazardBase::DamageTick, DamageInterval, true);
		}
	}
}

void ABsHazardBase::OnBoxComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IReceiveDamage* DamageReceiver = Cast<IReceiveDamage>(OtherActor))
	{
		OverlappingActors.Remove(DamageReceiver);
		if (OverlappingActors.IsEmpty())
		{
			DamageTimerHandle.Invalidate();
		}
	}
	
}


