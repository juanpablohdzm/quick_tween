// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickIntTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"

void UQuickIntTween::ApplyAlphaValue(float alpha)
{
	const int32 value = GetEaseCurve() ?
	FEaseFunctions<int32>::Ease(StartValue.Get(0), EndValue.Get(0), alpha, GetEaseCurve()) :
	FEaseFunctions<int32>::Ease(StartValue.Get(0), EndValue.Get(0), alpha, GetEaseType());
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

	if (!EndValue.IsSet())
	{
		if (!To.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickIntTween::HandleOnStartTransition: 'To' delegate is not bound."));
			return;
		}

		EndValue = To.Execute(this);
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

	const int32 value = bSnapToEnd ? EndValue.Get(0) : StartValue.Get(0);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}

	CurrentValue = value;

	Super::HandleOnComplete();
}
