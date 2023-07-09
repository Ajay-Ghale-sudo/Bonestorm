#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "BsCameraComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOOMERSHOOTER_API UBsCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBsCameraComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	/**
	 * @brief Add roll to the Camera
	 * @param Amount Amount of roll to add.
	 */
	void AddToRoll(float Amount);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	/**
	 * @brief The +/- amount the camera can lean.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Lean")
	float MaxRollAmount = 2.5f;

	/**
	 * @brief Speed of when lean is increasing.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Lean")
	float LeanInSpeed = 20.f;

	/**
	 * @brief Speed of lean returning to rest position.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Lean")
	float LeanOutSpeed = 5.f;
	
	float CurrentRollAmount = 0.f;
	float TargetRollAmount = 0.f;
	bool bRecentlyLeaned = false;
};