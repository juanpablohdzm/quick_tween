// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickVector2DTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"

void UQuickVector2DTween::ApplyAlphaValue(float alpha)
{
	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickVector2DTween::ApplyAlphaValue: 'To' delegate  is not bound, unable to interpolate."));
		return;
	}

	const FVector2D value = GetEaseCurve() ?
	FEaseFunctions<FVector2D>::Ease(StartValue.Get(FVector2D::ZeroVector), To.Execute(this), alpha, GetEaseCurve()) :
	FEaseFunctions<FVector2D>::Ease(StartValue.Get(FVector2D::ZeroVector), To.Execute(this), alpha, GetEaseType());
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}

	CurrentValue = value;
}

void UQuickVector2DTween::HandleOnStartTransition()
{

	if (!From.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickVector2DTween::HandleOnStartTransition: 'From' delegate is not bound."));
		return;
	}

	StartValue = From.Execute(this);

	Super::HandleOnStartTransition();
}

void UQuickVector2DTween::HandleOnCompleteTransition(bool bSnapToEnd)
{
	Super::HandleOnCompleteTransition(bSnapToEnd);

	if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
	{
		if (Setter.IsBound())
		{
			Setter.Execute(StartValue.Get(FVector2D::ZeroVector), this);
		}
		CurrentValue = StartValue.Get(FVector2D::ZeroVector);
		return;
	}

	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickVector2DTween::HandleOnCompleteTransition: 'To' delegate is not bound, unable to interpolate."));
		return;
	}

	const FVector2D value = bSnapToEnd ? To.Execute(this) : StartValue.Get(FVector2D::ZeroVector);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;
}
