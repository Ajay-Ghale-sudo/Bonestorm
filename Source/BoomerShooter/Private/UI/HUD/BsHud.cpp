// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BsHud.h"

#include "Blueprint/UserWidget.h"
#include "Character/BsCharacter.h"
#include "UI/Widget/BsDashAmountWidget.h"


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
	BindWidgets();
}

// Called every frame
void ABsHud::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABsHud::BindWidgets()
{
	if (DashAmountWidgetClass)
	{
		DashAmountWidget = Cast<UBsDashAmountWidget>(CreateWidget<UBsDashAmountWidget>(GetWorld(), DashAmountWidgetClass));
		if (DashAmountWidget && PlayerCharacter)
		{
			DashAmountWidget->AddToViewport();
			DashAmountWidget->SetOwningPlayer(GetOwningPlayerController());
			PlayerCharacter->OnDashAmountChanged.AddUObject(this, &ABsHud::UpdateDashAmount);
		}
	}
}

void ABsHud::UpdateDashAmount()
{
	if (DashAmountWidget && PlayerCharacter)
	{
		DashAmountWidget->SetDashAmount(PlayerCharacter->GetDashAmount());
	}
}