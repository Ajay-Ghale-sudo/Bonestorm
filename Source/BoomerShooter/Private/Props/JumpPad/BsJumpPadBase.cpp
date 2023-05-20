// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/JumpPad/BsJumpPadBase.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"


// Sets default values
ABsJumpPadBase::ABsJumpPadBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	LaunchDirection = CreateDefaultSubobject<UArrowComponent>("LaunchDirection");
	LaunchDirection->SetupAttachment(RootComponent);
	LaunchDirection->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void ABsJumpPadBase::BeginPlay()
{
	Super::BeginPlay();

	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABsJumpPadBase::OnSphereBeginOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABsJumpPadBase::OnSphereEndOverlap);
	}
}

void ABsJumpPadBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		JumpPadEffect(Character);
	}
}

void ABsJumpPadBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABsJumpPadBase::JumpPadEffect(ACharacter* Character)
{
	if (!Character || !bIsJumpPadActive) return;
	
	FVector LaunchVector = LaunchDirection->GetForwardVector() * JumpForce;
	Character->LaunchCharacter(LaunchVector, bOverrideXYVelocity, bOverrideZVelocity);
}

