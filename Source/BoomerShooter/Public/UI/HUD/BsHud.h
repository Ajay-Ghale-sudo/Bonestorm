// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/HUD.h"
#include "BsHud.generated.h"

class UBsCrosshairWidget;
class UBsDashAmountWidget;
class UBsHealthAmountWidget;
class ABsCharacter;
class UUserWidget;

UCLASS()
class BOOMERSHOOTER_API ABsHud : public AHUD
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABsHud();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitWidgets();
	
	UFUNCTION()
	void UpdateDashAmount();

	UFUNCTION()
	void UpdateDashCooldown();

	UFUNCTION()
	void RefreshDashWidget();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	ABsCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowedClasses = "BsDashAmountWidget"))
	TSubclassOf<UUserWidget> DashAmountWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UBsDashAmountWidget* DashAmountWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowedClasses = "BsHealthAmountWidget"))
	TSubclassOf<UUserWidget> HealthAmountWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UBsHealthAmountWidget* HealthAmountWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowedClasses = "BsCrosshairWidget"))
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "HUD")
	UBsCrosshairWidget* CrosshairWidget;

public:

};
