// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickColorTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"

void UQuickColorTween::ApplyAlphaValue(float alpha)
{
	const FColor startValue = StartValue.Get(FColor::White);
	const FColor endValue = EndValue.Get(FColor::White);
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

void UQuickColorTween::HandleOnStart()
{
	if (!StartValue.IsSet())
	{
		if (!From.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickColorTween::HandleOnStartTransition: 'From' delegate is not bound."));
			return;
		}
		StartValue = From.Execute(this);
	}

	if (!EndValue.IsSet())
	{
		if (!To.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickColorTween::HandleOnStartTransition: 'To' delegate is not bound."));
			return;
		}
		EndValue = To.Execute(this);
	}

	Super::HandleOnStart();
}

void UQuickColorTween::HandleOnComplete()
{
	if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
	{
		if (Setter.IsBound())
		{
			Setter.Execute(StartValue.Get(FColor::White), this);
		}
		CurrentValue = StartValue.Get(FColor::White);
		Super::HandleOnComplete();
		return;
	}

	bool bSnapToEnd  = GetSnapToEndOnComplete();
	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	const FColor value = bSnapToEnd ? EndValue.Get(FColor::White) : StartValue.Get(FColor::White);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;

	Super::HandleOnComplete();
}
