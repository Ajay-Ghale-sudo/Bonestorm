// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy/BsSkeleton.h"

#include "Components/BoxComponent.h"
#include "Data/AttackResult.h"
#include "Data/BsDamageType.h"
#include "Engine/DamageEvents.h"

// Sets default values
ABsSkeleton::ABsSkeleton()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeleeHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeHitBox"));
	MeleeHitBox->SetupAttachment(GetMesh(), TEXT("MeleeHitBoxSocket"));
}

// Called when the game starts or when spawned
void ABsSkeleton::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABsSkeleton::Die()
{
	Super::Die();
}

void ABsSkeleton::Attack()
{
	Super::Attack();
}

void ABsSkeleton::BindMeleeHitBox()
{
	if (MeleeHitBox)
	{
		MeleeHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABsSkeleton::OnMeleeHitBoxOverlap);
	}
}

void ABsSkeleton::OnMeleeHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->TakeDamage(MeleeAttackDamage, FDamageEvent(UBsSkeletonMeleeDamageType::StaticClass()), GetController(), this);
	}
}

EAttackResult ABsSkeleton::MeleeAttack()
{
	if (!CanAttack()) return EAttackResult::EAR_None;

	PlayMontage(MeleeAttackMontage);
	OnAttack();
	return EAttackResult::EAR_Successful;
}