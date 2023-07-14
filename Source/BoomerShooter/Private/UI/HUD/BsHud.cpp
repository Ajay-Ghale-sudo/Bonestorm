// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BsHud.h"

#include "Blueprint/UserWidget.h"
#include "Character/BsCharacter.h"
#include "Component/BsHealthComponent.h"
#include "Controller/BsPlayerController.h"
#include "UI/Widget/BsCrosshairWidget.h"
#include "UI/Widget/BsDashAmountWidget.h"
#include "UI/Widget/BsHealthAmountWidget.h"
#include "UI/Widget/BsSeveredHeadWidget.h"
#include "UI/Widget/Indicator/BsDamageIndicatorWidget.h"
#include "UI/Widget/Indicator/BsLowHealthIndicatorWidget.h"
#include "UI/Widget/Menu/BsStartMenuWidget.h"
#include "UI/Widget/Notification/BsNotificationWidget.h"
#include "Weapon/BsWeaponBase.h"


// Sets default values
ABsHud::ABsHud()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ABsHud::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void ABsHud::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ABsCharacter>(GetOwningPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->OnPaused.AddDynamic(this, &ABsHud::OnPause);
		PlayerCharacter->OnUnpaused.AddDynamic(this, &ABsHud::OnUnpause);
		PlayerCharacter->OnWeaponChanged.AddUObject(this, &ABsHud::OnWeaponChanged);
	}
	
	InitWidgets();
	AddPlayerWidgetsToViewport();
	ShowPlayerWidgets(true);
}

void ABsHud::OnPause()
{
	AddStartMenuWidgetToViewport();
	ShowStartMenu(true);
	ShowPlayerWidgets(false);
}

void ABsHud::OnUnpause()
{
	ShowStartMenu(false);
	AddPlayerWidgetsToViewport();
	ShowPlayerWidgets(true);
}

void ABsHud::AddStartMenuWidgetToViewport()
{
	if (StartMenuWidget)
	{
		StartMenuWidget->AddToViewport();
	}
}

void ABsHud::AddPlayerWidgetsToViewport()
{
	if (DashAmountWidget)
	{
		DashAmountWidget->AddToViewport();
	}
	
	if (HealthAmountWidget)
	{
		HealthAmountWidget->AddToViewport();
	}
	
	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
	}

	if (DamageIndicatorWidget)
	{
		DamageIndicatorWidget->AddToViewport();
	}

	if (LowHealthIndicatorWidget)
	{
		LowHealthIndicatorWidget->AddToViewport(-1);
	}
}

void ABsHud::ShowStartMenu(bool bShow)
{
	if (StartMenuWidget)
	{
		StartMenuWidget->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void ABsHud::ShowPlayerWidgets(bool bShow)
{
	const ESlateVisibility Visibility = bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	if (DashAmountWidget)
	{
		DashAmountWidget->SetVisibility(Visibility);
	}
	
	if (HealthAmountWidget)
	{
		HealthAmountWidget->SetVisibility(Visibility);
	}
	
	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(Visibility);
	}

	if (DamageIndicatorWidget)
	{
		DamageIndicatorWidget->SetVisibility(Visibility);
	}

	if (LowHealthIndicatorWidget)
	{
		LowHealthIndicatorWidget->SetVisibility(Visibility);
	}
}

void ABsHud::AddNotification(const FText& Text, TSubclassOf<UBsNotificationWidget> NotificationClass)
{
	if (UBsNotificationWidget* NotificationWidget = CreateWidget<UBsNotificationWidget>(GetWorld(), NotificationClass ? NotificationClass : NotificationWidgetClass))
	{
		NotificationWidget->AddToViewport();
		NotificationWidget->ShowNotification(Text);
	}
}

void ABsHud::InitWidgets()
{

	if (StartMenuWidgetClass)
	{
		StartMenuWidget = CreateWidget<UBsStartMenuWidget>(GetWorld(), StartMenuWidgetClass);
		if (StartMenuWidget && PlayerCharacter)
		{
			StartMenuWidget->OnStart.AddDynamic(PlayerCharacter, &ABsCharacter::Unpause);
			if (ABsPlayerController* PC = Cast<ABsPlayerController>(GetOwningPlayerController()))
			{
				StartMenuWidget->OnRestart.AddDynamic(PC, &ABsPlayerController::RestartLevel);
				StartMenuWidget->OnQuit.AddDynamic(PC, &ABsPlayerController::Quit);
			}
		}
	}
	
	if (DashAmountWidgetClass)
	{
		DashAmountWidget = CreateWidget<UBsDashAmountWidget>(GetWorld(), DashAmountWidgetClass);
		if (DashAmountWidget && PlayerCharacter)
		{
			DashAmountWidget->SetOwningPlayer(GetOwningPlayerController());
			PlayerCharacter->OnDashAmountChanged.AddUObject(this, &ABsHud::UpdateDashAmount);
			PlayerCharacter->OnDashEnabledChanged.AddUObject(this, &ABsHud::UpdateDashCooldown);
			// TODO: Find a method of solving race condition
			GetWorldTimerManager().SetTimerForNextTick(this, &ABsHud::RefreshDashWidget);
		}
	}

	if (CrosshairWidgetClass)
	{
		CrosshairWidget = CreateWidget<UBsCrosshairWidget>(GetWorld(), CrosshairWidgetClass);
	}
	
	if (HealthAmountWidgetClass && PlayerCharacter)
	{
		HealthAmountWidget = CreateWidget<UBsHealthAmountWidget>(GetWorld(), HealthAmountWidgetClass);
	}

	if (DamageIndicatorWidgetClass)
	{
		DamageIndicatorWidget = CreateWidget<UBsDamageIndicatorWidget>(GetWorld(), DamageIndicatorWidgetClass);
	}

	if (SeveredHeadWidgetClass)
	{
		SeveredHeadWidget = CreateWidget<UBsSeveredHeadWidget>(GetWorld(), SeveredHeadWidgetClass);
	}

	if (LowHealthIndicatorWidgetClass)
	{
		LowHealthIndicatorWidget = CreateWidget<UBsLowHealthIndicatorWidget>(GetWorld(), LowHealthIndicatorWidgetClass);
	}
}

void ABsHud::UpdateDashAmount()
{
	if (DashAmountWidget && PlayerCharacter)
	{
		DashAmountWidget->SetDashAmount(PlayerCharacter->GetDashAmount());
	}
}

void ABsHud::UpdateDashCooldown()
{
	if (DashAmountWidget && PlayerCharacter)
	{
		DashAmountWidget->SetDashEnabled(PlayerCharacter->GetDashEnabled());
	}
}

void ABsHud::RefreshDashWidget()
{
	UpdateDashAmount();
	UpdateDashCooldown();
}

void ABsHud::OnWeaponChanged(ABsWeaponBase* NewWeapon)
{
	OnWeaponChangedHandle.Reset();
	if (NewWeapon)
	{
		OnWeaponChangedHandle = NewWeapon->OnWeaponHeadAttached.AddUObject(this, &ABsHud::OnWeaponHeadAttached);
	}
}

void ABsHud::OnWeaponHeadAttached(ABsSeveredHeadBase* SeveredHead)
{
	if (SeveredHead && SeveredHeadWidget)
	{
		SeveredHeadWidget->SetSeveredHead(SeveredHead);
		SeveredHeadWidget->AddToViewport();
		SeveredHeadWidget->SetVisibility(ESlateVisibility::Visible);
	}
}