// Copyright Epic Games, Inc. All Rights Reserved.


#include "BoomerShooterGameModeBase.h"

#include "Character/BsCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/BsWeaponBase.h"

void ABoomerShooterGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	InitCharacter();
}

void ABoomerShooterGameModeBase::InitCharacter()
{
	if (DefaultWeaponClass)
	{
		if (ABsCharacter* Character = Cast<ABsCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
		{
			if (Character->GetWeapon()) return; //Already have a weapon equipped.

			if (ABsWeaponBase* Weapon = GetWorld()->SpawnActor<ABsWeaponBase>(DefaultWeaponClass))
			{
				Character->SetWeapon(Weapon);
			}
		}
	}
}
