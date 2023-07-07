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
}

// Called every frame
void UBsCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType,
									   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float Target = 0.f;
	float Speed = LeanOutSpeed;
	
	if (bRecentlyLeaned)
	{
		bRecentlyLeaned = false;
		Target = TargetRollAmount;
		Speed = LeanInSpeed;
	}
	
	CurrentRollAmount = FMath::FInterpTo(CurrentRollAmount, Target, DeltaTime, Speed);
}

void UBsCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);

	// Override the Controller roll to match the player's lean
	const APawn* OwningPawn = Cast<APawn>(GetOwner());
	const AController* OwningController = OwningPawn ? OwningPawn->GetController() : nullptr;
	if (OwningController && OwningController->IsLocalPlayerController())
	{
		FRotator PawnViewRotation = OwningPawn->GetViewRotation();
		PawnViewRotation.Roll = CurrentRollAmount;
		if (!PawnViewRotation.Equals(GetComponentRotation()))
		{
			SetWorldRotation(PawnViewRotation);
		}
	}
}

void UBsCameraComponent::AddToRoll(float Amount)
{
	// If Amount is nearly zero then we aren't leaning
	if (FMath::IsNearlyZero(Amount)) return;
	
	TargetRollAmount =  FMath::Clamp(CurrentRollAmount + Amount, -MaxRollAmount, MaxRollAmount); 
	bRecentlyLeaned = true;
}