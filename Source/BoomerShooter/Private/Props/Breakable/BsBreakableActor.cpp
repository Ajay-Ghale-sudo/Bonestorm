#include "Props/Breakable/BsBreakableActor.h"
#include "Component/BsHealthComponent.h"

// Sets default values
ABsBreakableActor::ABsBreakableActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);

	HealthComponent = CreateDefaultSubobject<UBsHealthComponent>("HealthComponent");
}

// Called when the game starts or when spawned
void ABsBreakableActor::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ABsBreakableActor::Break);
	}
}

void ABsBreakableActor::Break()
{
	OnBreak.Broadcast();
}