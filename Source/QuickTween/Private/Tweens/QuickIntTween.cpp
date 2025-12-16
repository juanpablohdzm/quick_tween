// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickIntTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"

void UQuickIntTween::ApplyAlphaValue(float alpha)
{
	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickIntTween::ApplyAlphaValue: 'To' delegate is not bound, unable to interpolate."));
		return;
	}

	const int32 value = GetEaseCurve() ?
	FEaseFunctions<int32>::Ease(StartValue.Get(0), To.Execute(this), alpha, GetEaseCurve()) :
	FEaseFunctions<int32>::Ease(StartValue.Get(0), To.Execute(this), alpha, GetEaseType());
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}

	CurrentValue = value;
}

void UQuickIntTween::HandleOnStart()
{
	if (!StartValue.IsSet())
	{
		if (!From.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickIntTween::HandleOnStartTransition: 'From' delegate is not bound."));
			return;
		}

		StartValue = From.Execute(this);
	}

	Super::HandleOnStart();
}

void UQuickIntTween::HandleOnComplete()
{
	if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
	{
		if (Setter.IsBound())
		{
			Setter.Execute(StartValue.Get(0), this);
		}
		CurrentValue = StartValue.Get(0);
		Super::HandleOnComplete();
		return;
	}

	bool bSnapToEnd = GetSnapToEndOnComplete();
	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickIntTween::HandleOnCompleteTransition: 'To' delegate is not bound, unable to complete tween."));
		return;
	}

	const int32 value = bSnapToEnd ? To.Execute(this) : StartValue.Get(0);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}

	CurrentValue = value;

	Super::HandleOnComplete();
}
