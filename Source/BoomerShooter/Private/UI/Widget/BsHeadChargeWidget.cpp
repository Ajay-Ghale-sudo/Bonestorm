// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BsHeadChargeWidget.h"
#include "Props/Head/BsSeveredHeadBase.h"

void UBsHeadChargeWidget::BindToSeveredHead(ABsSeveredHeadBase* SeveredHead)
{
	if (SeveredHead)
	{
		AttachedHead = SeveredHead;
		AttachedHead->OnHeadChargeChanged.AddUObject(this, &UBsHeadChargeWidget::RefreshWidget);
		RefreshWidget();
	}
}

void UBsHeadChargeWidget::RefreshWidget()
{
	if (AttachedHead)
	{
		UpdateHeadCharge(AttachedHead->GetCurrentCharge());
	}
}

void UBsHeadChargeWidget::HideWidget_Implementation()
{
	
}

void UBsHeadChargeWidget::UpdateHeadCharge_Implementation(float HeadCharge)
{
	
}