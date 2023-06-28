// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Head/BsSeveredHeadBase.h"
#include "Weapon/Projectile/BsProjectileBase.h"
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

	PopHead();
}

void ABsSeveredHeadBase::SetAttached(bool bAttached)
{
	bIsAttached = bAttached;

	if (bIsAttached)
	{
		DisableMeshOverlap();
		ShowWidget();
	}
	else 
	{
		HideWidget();
		
		// Only enable overlap if we have charge.
		if (CurrentCharge > 0.f)
		{
			EnableMeshOverlap();
		}
	}
}

void ABsSeveredHeadBase::ShowWidget() const
{
	if (HeadWidgetComponent)
	{
		HeadWidgetComponent->SetVisibility(true);
	}
}

void ABsSeveredHeadBase::HideWidget() const
{
	if (HeadWidgetComponent)
	{
		HeadWidgetComponent->SetVisibility(false);
	}
}

void ABsSeveredHeadBase::AttachHeadToComponent(USceneComponent* InParent, const FName& SocketName)
{
	if (!InParent || !HeadMesh) return;

	SetOwner(InParent->GetOwner());
	HeadMesh->SetSimulatePhysics(false);
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	SetActorScale3D(SeveredHeadScale);
	SetAttached(true);
}

void ABsSeveredHeadBase::DetachHead()
{
	if (!bIsAttached) return; // Already detached.
	
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetOwner(nullptr);
	if (HeadMesh)
	{
		HeadMesh->SetSimulatePhysics(true);
		HeadMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	SetAttached(false);
	PopHead();

	OnDetachedHead.Broadcast();
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
	// Parrying increases charge
	DepleteCharge(Damage * -ParryingChargeMultiplier);
	return Damage;
}

float ABsSeveredHeadBase::BlockDamage(float Damage)
{
	DepleteCharge(Damage * BlockChargeMultiplier);
	return Damage * BlockDamageReductionMultiplier;
}

float ABsSeveredHeadBase::Consume()
{
	if (MaxCharge <= 0.f) return 0.f;

	// Amount to heal based on % of charge.
	const float AmountToHeal = (CurrentCharge / MaxCharge) * HealingAmount;
	// Deplete all charge
	DepleteCharge(CurrentCharge);
	
	return AmountToHeal;
}

void ABsSeveredHeadBase::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	
}

void ABsSeveredHeadBase::PopHead() const
{
	if (!HeadMesh || bIsAttached) return;
	HeadMesh->AddImpulse(FVector(0.f, 0.f, 500.f), NAME_None, true);
}

void ABsSeveredHeadBase::DepleteCharge()
{
	DepleteCharge(ChargeCost);
}

void ABsSeveredHeadBase::DepleteCharge(const float Amount)
{
	CurrentCharge = FMath::Clamp(CurrentCharge - Amount, 0.f, MaxCharge);
	OnHeadChargeChanged.Broadcast();

	if (CurrentCharge <= 0.f)
	{
		DetachHead();
		
		// TODO: This shouldn't destroy explicitly. Needs to be resolved elsewhere where it can be overridden for fx to be played.
		SetLifeSpan(2.f);
	}
}

ABsProjectileBase* ABsSeveredHeadBase::CreateProjectile(TSubclassOf<ABsProjectileBase> ProjectileClass, const FTransform &SpawnTransform,
                                                        FActorSpawnParameters& SpawnParameters)
{
    UWorld* World = GetWorld();
    if (ProjectileClass && World)
    {
    	if (CurrentCharge > 0.f)
    	{
    		DepleteCharge();
    		OnHeadChargeChanged.Broadcast();
		    if (ABsProjectileBase* Projectile =  Cast<ABsProjectileBase>(World->SpawnActor(HeadProjectileClass ? HeadProjectileClass : ProjectileClass, &SpawnTransform, SpawnParameters)))
		    {
    			Projectile->SetDamageType(HeadDamageType);
		    	return Projectile;
		    }
    	}
	}
	return nullptr;
}