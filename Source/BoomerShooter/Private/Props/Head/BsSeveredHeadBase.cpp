// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Head/BsSeveredHeadBase.h"
#include "Weapon/Projectile/BsProjectileBase.h"
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

	EnableMeshOverlap();	
}

void ABsSeveredHeadBase::SetAttached(bool bAttached)
{
	bIsAttached = bAttached;

	if (bIsAttached)
	{
		DisableMeshOverlap();
	}
	else
	{
		EnableMeshOverlap();
	}
}

void ABsSeveredHeadBase::EnableMeshOverlap()
{
	if (HeadMesh)
	{
		HeadMesh->OnComponentBeginOverlap.AddDynamic(this, &ABsSeveredHeadBase::OnMeshOverlapBegin);
		HeadMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ABsSeveredHeadBase::DisableMeshOverlap()
{
	if (HeadMesh)
	{
		HeadMesh->OnComponentBeginOverlap.RemoveDynamic(this, &ABsSeveredHeadBase::OnMeshOverlapBegin);
		HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABsSeveredHeadBase::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	
}

ABsProjectileBase* ABsSeveredHeadBase::CreateProjectile(TSubclassOf<ABsProjectileBase> ProjectileClass, const FTransform &SpawnTransform,
	FActorSpawnParameters& SpawnParameters)
{
	ABsProjectileBase* Projectile = nullptr;
	UWorld* World = GetWorld();
	if (ProjectileClass && World)
	{
		Projectile = Cast<ABsProjectileBase>(World->SpawnActor(ProjectileClass ,&SpawnTransform, SpawnParameters));
		if (Projectile)
		{
			Projectile->SetDamageType(HeadDamageType);
		}
	}
	return Projectile;
}