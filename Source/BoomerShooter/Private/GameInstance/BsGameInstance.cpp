// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/BsGameInstance.h"

#include "FxManager/BsImpactFxManager.h"

void UBsGameInstance::Init()
{
	Super::Init();

	if (ImpactFxManagerClass)
	{
		ImpactFxManager = NewObject<UBsImpactFxManager>(this, ImpactFxManagerClass);
		if (ImpactFxManager)
		{
			ImpactFxManager->Init();
		}
	}
}