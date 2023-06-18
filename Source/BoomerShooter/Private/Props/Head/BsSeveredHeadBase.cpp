// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Head/BsSeveredHeadBase.h"
#include "Weapon/Projectile/BsProjectileBase.h"
#include "Component/BsInventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/BsHeadChargeWidget.h"


// Sets default values
ABsSeveredHeadBase::ABsSeveredHeadBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetSimulatePhysics(true);
	SetRootComponent(HeadMesh);
	HeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	HeadWidgetComponent->SetVisibility(false);
	HeadWidgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABsSeveredHeadBase::BeginPlay()
{
	Super::BeginPlay();

	EnableMeshOverlap();
	CurrentCharge = MaxCharge;
	if (UBsHeadChargeWidget* Widget = Cast<UBsHeadChargeWidget>(HeadWidgetComponent->GetWidget()))
	{
		Widget->BindToSeveredHead(this);
	}
}

void ABsSeveredHeadBase::SetAttached(bool bAttached)
{
	bIsAttached = bAttached;

	if (bIsAttached)
	{
		DisableMeshOverlap();
		if (HeadWidgetComponent)
		{
			HeadWidgetComponent->SetVisibility(true);
		}
	}
	else
	{
		EnableMeshOverlap();
		HeadWidgetComponent->SetVisibility(false);
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

float ABsSeveredHeadBase::ParryDamage(float Damage)
{
	CurrentCharge = FMath::Clamp(CurrentCharge + Damage * ParryingChargeMultiplier, 0, MaxCharge);
	OnHeadChargeChanged.Broadcast();
	return Damage;
}

float ABsSeveredHeadBase::BlockDamage(float Damage)
{
	CurrentCharge = FMath::Clamp(CurrentCharge - Damage * BlockChargeMultiplier, 0, MaxCharge);
	if (CurrentCharge <= 0.f)
	{
		OnDetachedHead.Broadcast();
		return Damage;
	}
	OnHeadChargeChanged.Broadcast();
	return Damage;
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
    	if (CurrentCharge > 0.f)
    	{
    		CurrentCharge = FMath::Clamp(CurrentCharge - ChargeCost, 0, MaxCharge);
    		Projectile = Cast<ABsProjectileBase>(World->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParameters));
    		OnHeadChargeChanged.Broadcast();
		    if (Projectile)
		    {
    			Projectile->SetDamageType(HeadDamageType);
		    }
    	}
    	if (CurrentCharge <= 0.f)
    	{
    		OnDetachedHead.Broadcast();
    	}
	}
	return Projectile;
}