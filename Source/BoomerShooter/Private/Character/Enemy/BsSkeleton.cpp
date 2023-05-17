// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BsSkeleton.h"

#include "Components/CapsuleComponent.h"


// Sets default values
ABsSkeleton::ABsSkeleton()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABsSkeleton::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABsSkeleton::TriggerRagdoll()
{
	if (USkeletalMeshComponent* CurrentMesh = GetMesh())
	{
		
		CurrentMesh->SetSimulatePhysics(true);
		CurrentMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		CurrentMesh->SetAllBodiesBelowSimulatePhysics(FName("Pelvis"), true, true);
		CurrentMesh->SetCollisionResponseToAllChannels(ECR_Block);
		CurrentMesh->SetSimulatePhysics(true);
		CurrentMesh->SetPhysicsBlendWeight(0.75f);
		//CurrentMesh->WakeAllRigidBodies();
	}
	if (UCapsuleComponent* CurrentCapsule = GetCapsuleComponent())
	{
		CurrentCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABsSkeleton::Die()
{
	Super::Die();
	TriggerRagdoll();
}

// Called every frame
void ABsSkeleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
