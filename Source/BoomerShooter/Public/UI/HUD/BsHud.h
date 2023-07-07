// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/HUD.h"
#include "BsHud.generated.h"

class UBsStartMenuWidget;
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

	UFUNCTION(BlueprintCallable)
	void AddStartMenuWidgetToViewport();
	
	UFUNCTION(BlueprintCallable)
	void AddPlayerWidgetsToViewport();

	UFUNCTION(BlueprintCallable)
	void ShowStartMenu(bool bShow);
	
	UFUNCTION(BlueprintCallable)
	void ShowPlayerWidgets(bool bShow);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPause();

	UFUNCTION()
	void OnUnpause();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowedClasses = "BsStartMenuWidget"))
	TSubclassOf<UUserWidget> StartMenuWidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "HUD")
	UBsStartMenuWidget* StartMenuWidget;
};
