#include "Component/Camera/BsCameraComponent.h"

// Sets default values for this component's properties
UBsCameraComponent::UBsCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UBsCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	InitialFOV = FieldOfView;
	TargetFOV = InitialFOV;
}

// Called every frame
void UBsCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType,
									   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FOVTick(DeltaTime);

	float Target = 0.f;
	float Speed = LeanOutSpeed;
	
	if (bRecentlyLeaned)
	{
		bRecentlyLeaned = false;
		Target = TargetRollAmount;
		Speed = LeanInSpeed;
	}
	
	CurrentRollAmount = FMath::FInterpTo(CurrentRollAmount, Target, DeltaTime, Speed);
	SetRelativeRotation(FRotator(0.f, 0.f, CurrentRollAmount));
}

void UBsCameraComponent::AddToRoll(float Amount)
{
	// If Amount is nearly zero then we aren't leaning
	if (FMath::IsNearlyZero(Amount)) return;
	
	TargetRollAmount =  FMath::Clamp(CurrentRollAmount + Amount, -MaxRollAmount, MaxRollAmount); 
	bRecentlyLeaned = true;
}

void UBsCameraComponent::StartDashFOV()
{
	SetTargetFOV(DashFOV);
}

void UBsCameraComponent::StartSlideFOV()
{
	SetTargetFOV(SlideFOV);
}

void UBsCameraComponent::ResetFOV()
{
	SetTargetFOV(InitialFOV);
}

void UBsCameraComponent::SetTargetFOV(const float InTargetFOV)
{
	TargetFOV = InTargetFOV;
}

void UBsCameraComponent::FOVTick(const float DeltaTime)
{
	if (!FMath::IsNearlyEqual(FieldOfView, TargetFOV,  0.01f))
	{
		FieldOfView = FMath::FInterpTo(FieldOfView, TargetFOV, DeltaTime, FOVSpeed);
	}
}
