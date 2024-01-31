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
	
	/**
	 * @brief Add roll to the Camera
	 * @param Amount Amount of roll to add.
	 */
	void AddToRoll(float Amount);

	/**
	 * @brief Sets TargetFOV to DashFOV.
	 */
	UFUNCTION()
	void StartDashFOV();

	/**
	 * @brief Sets TargetFOV to SlideFOV.
	 */
	UFUNCTION()
	void StartSlideFOV();

	/**
	 * @brief Resets TargetFOV to InitialFOV.
	 */
	UFUNCTION()
	void ResetFOV();

	/**
	 * @brief Sets TargetFOV to InTargetFOV.
	 * @param InTargetFOV The new TargetFOV.
	 */
	void SetTargetFOV(const float InTargetFOV);

	/**
	 * @brief Apply the Speed Lines Post Process effect.
	 */
	UFUNCTION()
	void ApplySpeedLines();

	/**
	 * @brief Remove the Speed Lines Post Process effect.
	 */
	UFUNCTION()
	void RemoveSpeedLines();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * @brief Tick function for FOV. Interpolates FOV to TargetFOV.
	 * @param DeltaTime The time since the last tick.
	 */
	UFUNCTION()
	void FOVTick(const float DeltaTime);

	/**
	 * @brief Tick function for applying post process effects.
	 * @param DeltaTime The time since last tick.
	 */
	void PostProcessTick(const float DeltaTime);
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
	float LeanOutSpeed = 5;

	/**
	 * @brief Float to control the exposure of the camera
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	float ManualExposureCompensation = 8.5;
	
	float CurrentRollAmount = 0.f;
	float TargetRollAmount = 0.f;
	bool bRecentlyLeaned = false;

	/**
	 * @brief The initial FOV of the camera.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera|FOV")
	float InitialFOV;

	/**
	 * @brief The target FOV of the camera.
	 */
	float TargetFOV;

	/**
	 * @brief Adjustable offset for the FOV while dashing. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|FOV")
	float FOVDashOffset = 25.0f;

	/**
	 * @brief Adjustable offset for the FOV while sliding.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|FOV")
	float FOVSlideOffset = 15.0f;

	/**
	 * @brief The speed of the FOV interpolation.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|FOV")
	float FOVSpeed = 5.f;

	/**
	 * @brief Post Process Material to apply when Dash/Sliding.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Effects")
	TObjectPtr<UMaterialInstance> Material_SpeedLines;

	/**
	 * @brief Dynamic Instance of SpeedLines material.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Camera|Effects")
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance_SpeedLines;

	/**
	 * @brief The weight of the Speed Lines Post Process effect.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Effects")
	float SpeedLinesWeight = 1.f;

	/**
	 * @brief The desired value of SpeedLineOpacity.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Effects")
	float TargetSpeedLineOpacity = 1.f;

	/**
	 * @brief Current Opacity value to set the SpeedLines material.
	 */
	float SpeedLineOpacity = 0.f;

	/**
	 * @brief The value to start the SpeedLineOpacity at. Used in ApplySpeedLines()
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Effects")
	float StartingSpeedLineOpacity = 0.5f; 

	/**
	 * @brief The speed at which the SpeedLine opacity should change. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Effects")
	float SpeedLineOpacitySpeed = 5.f;
};