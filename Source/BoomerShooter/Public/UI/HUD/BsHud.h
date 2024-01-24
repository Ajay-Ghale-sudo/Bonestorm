// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "BsHud.generated.h"

class UBsLowHealthIndicatorWidget;
class ABsSeveredHeadBase;
class UBsSeveredHeadWidget;
class ABsWeaponBase;
class UBsNotificationWidget;
class UBsDamageIndicatorWidget;
class UBsStartMenuWidget;
class UBsCrosshairWidget;
class UBsDashAmountWidget;
class UBsHealthAmountWidget;
class ABsCharacter;
class UUserWidget;


USTRUCT(BlueprintType)
struct FBsGameplayWidgetConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UUserWidget> WidgetInstance;	
};

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

	UFUNCTION(BlueprintCallable)
	void AddNotification(const FText& Text, TSubclassOf<UBsNotificationWidget> NotificationClass = nullptr);

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

	UFUNCTION()
	void OnWeaponChanged(ABsWeaponBase* NewWeapon);

	UFUNCTION()
	void OnWeaponHeadAttached(ABsSeveredHeadBase* SeveredHead);

	/**
	 * @brief Called when a UI event is triggered
	 * @param EventTag A tag that identifies the event
	 */
	UFUNCTION()
	void OnUIEventTriggered(const FGameplayTag& EventTag);


	/**
	 * @brief Called when a UI event is ended
	 * @param EventTag A tag that identifies the event
	 */
	UFUNCTION()
	void OnUIEventEnded(const FGameplayTag& EventTag);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	ABsCharacter* PlayerCharacter;

	/**
	 * @brief Map of GameplayTags to Widget configurations.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TMap<FGameplayTag, FBsGameplayWidgetConfig> UIMessageWidgetMap;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowedClasses = "BsDamageIndicatorWidget"))
	TSubclassOf<UBsDamageIndicatorWidget> DamageIndicatorWidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "HUD")
	UBsDamageIndicatorWidget* DamageIndicatorWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowedClasses = "BsNotificationWidget"))
	TSubclassOf<UBsNotificationWidget> NotificationWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowedClasses = "BsSeveredHeadWidget"))
	TSubclassOf<UBsSeveredHeadWidget> SeveredHeadWidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "HUD")
	UBsSeveredHeadWidget* SeveredHeadWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowedClasses = "BsLowHealthIndicatorWidget"))
	TSubclassOf<UBsLowHealthIndicatorWidget> LowHealthIndicatorWidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "HUD")
	UBsLowHealthIndicatorWidget* LowHealthIndicatorWidget;

	FDelegateHandle OnWeaponChangedHandle;
};
