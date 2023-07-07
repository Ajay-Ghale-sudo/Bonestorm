// Fill out your copyright notice in the Description page of Project Settings.

#include "Controller/BsPlayerController.h"

#include "Character/BsCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABsPlayerController::ABsPlayerController()
{

}

void ABsPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

bool ABsPlayerController::SetPause(bool bPause, FCanUnpause CanUnpauseDelegate)
{
	const bool bResult = Super::SetPause(bPause, CanUnpauseDelegate);
	if (bPause)
	{
		OnPause();
	}
	else
	{
		OnUnpause();
	}
	return bResult;
}

void ABsPlayerController::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}

void ABsPlayerController::OnPause()
{
	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);
}

void ABsPlayerController::OnUnpause()
{
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}