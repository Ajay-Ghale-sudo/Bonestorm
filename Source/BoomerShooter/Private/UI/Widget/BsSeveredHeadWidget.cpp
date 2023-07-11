// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BsSeveredHeadWidget.h"

#include "Props/Head/BsSeveredHeadBase.h"

void UBsSeveredHeadWidget::SetSeveredHead(ABsSeveredHeadBase* InSeveredHead)
{

	if (SeveredHead)
	{
		SeveredHead->OnDetachedHead.RemoveAll(this);
		SeveredHead->OnHeadChargeChanged.RemoveAll(this);
	}
	
	SeveredHead = InSeveredHead;

	if (SeveredHead)
	{
		SeveredHead->OnDetachedHead.AddUObject(this, &UBsSeveredHeadWidget::ClearSeveredHead);
		SeveredHead->OnHeadChargeChanged.AddUObject(this, &UBsSeveredHeadWidget::HeadChargeChanged);
	}
	HeadAttached();
}

void UBsSeveredHeadWidget::ClearSeveredHead()
{
	SetSeveredHead(nullptr);
}

void UBsSeveredHeadWidget::HeadChargeChanged()
{
	if (SeveredHead)
	{
		UpdateHeadCharge(SeveredHead->GetCurrentCharge());
	}
}

void UBsSeveredHeadWidget::HeadAttached_Implementation()
{
}

void UBsSeveredHeadWidget::UpdateHeadCharge_Implementation(const float NewCharge)
{
}