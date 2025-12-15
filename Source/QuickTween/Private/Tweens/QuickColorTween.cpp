// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickColorTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"

void UQuickColorTween::ApplyAlphaValue(float alpha)
{
	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickColorTween::ApplyAlphaValue: 'To' delegate is not bound, unable to interpolate."));
		return;
	}

	const FColor startValue = StartValue.Get(FColor::White);
	const FColor endValue = To.Execute(this);
	FColor value;
	if (GetEaseCurve())
	{
		value.R = FEaseFunctions<uint8>::Ease(startValue.R, endValue.R, alpha, GetEaseCurve());
		value.G = FEaseFunctions<uint8>::Ease(startValue.G, endValue.G, alpha, GetEaseCurve());
		value.B = FEaseFunctions<uint8>::Ease(startValue.B, endValue.B, alpha, GetEaseCurve());
		value.A = FEaseFunctions<uint8>::Ease(startValue.A, endValue.A, alpha, GetEaseCurve());
	}
	else
	{
		value.R = FEaseFunctions<uint8>::Ease(startValue.R, endValue.R, alpha, GetEaseType());
		value.G = FEaseFunctions<uint8>::Ease(startValue.G, endValue.G, alpha, GetEaseType());
		value.B = FEaseFunctions<uint8>::Ease(startValue.B, endValue.B, alpha, GetEaseType());
		value.A = FEaseFunctions<uint8>::Ease(startValue.A, endValue.A, alpha, GetEaseType());
	}
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;
}

void UQuickColorTween::HandleOnStartTransition()
{
	if (!From.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickColorTween::HandleOnStartTransition: 'From' delegate is not bound."));
		return;
	}
	StartValue = From.Execute(this);

	Super::HandleOnStartTransition();
}

void UQuickColorTween::HandleOnCompleteTransition(bool bSnapToEnd)
{
	Super::HandleOnCompleteTransition(bSnapToEnd);

	if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
	{
		if (Setter.IsBound())
		{
			Setter.Execute(StartValue.Get(FColor::White), this);
		}
		CurrentValue = StartValue.Get(FColor::White);
		return;
	}

	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickColorTween::HandleOnCompleteTransition: 'To' delegate is not bound, unable to complete tween."));
		return;
	}

	const FColor value = bSnapToEnd ? To.Execute(this) : StartValue.Get(FColor::White);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;
}
