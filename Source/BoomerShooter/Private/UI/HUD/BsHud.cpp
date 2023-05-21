// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BsHud.h"

#include "Blueprint/UserWidget.h"
#include "Character/BsCharacter.h"
#include "UI/Widget/BsCrosshairWidget.h"
#include "UI/Widget/BsDashAmountWidget.h"
#include "UI/Widget/BsHealthAmountWidget.h"


// Sets default values
ABsHud::ABsHud()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABsHud::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ABsCharacter>(GetOwningPawn());
	InitWidgets();
}

// Called every frame
void ABsHud::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABsHud::InitWidgets()
{
	if (DashAmountWidgetClass)
	{
		DashAmountWidget = CreateWidget<UBsDashAmountWidget>(GetWorld(), DashAmountWidgetClass);
		if (DashAmountWidget && PlayerCharacter)
		{
			DashAmountWidget->AddToViewport();
			DashAmountWidget->SetOwningPlayer(GetOwningPlayerController());
			PlayerCharacter->OnDashAmountChanged.AddUObject(this, &ABsHud::UpdateDashAmount);
		}
	}

	if (CrosshairWidgetClass)
	{
		CrosshairWidget = CreateWidget<UBsCrosshairWidget>(GetWorld(), CrosshairWidgetClass);
		CrosshairWidget->AddToViewport();
	}
	if (HealthAmountWidget)
	{
		HealthAmountWidget = CreateWidget<UBsHealthAmountWidget>(GetWorld(), HealthAmountWidgetClass);
		HealthAmountWidget->AddToViewport();
	}
}

void ABsHud::UpdateDashAmount()
{
	if (DashAmountWidget && PlayerCharacter)
	{
		DashAmountWidget->SetDashAmount(PlayerCharacter->GetDashAmount());
	}
}