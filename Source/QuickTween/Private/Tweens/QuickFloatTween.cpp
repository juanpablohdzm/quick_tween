// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickFloatTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"

void UQuickFloatTween::ApplyAlphaValue(float alpha)
{
	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickFloatTween::ApplyAlphaValue: 'To' delegate is not bound, unable to interpolate."));
		return;
	}

	const float value = FEaseFunctions<float>::Ease(StartValue.Get(0.0f), To.Execute(this), alpha, GetEaseType());
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;
}

void UQuickFloatTween::HandleOnStartTransition()
{

	if (!From.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickFloatTween::HandleOnStartTransition: 'From' delegate is not bound."));
		return;
	}
	StartValue = From.Execute(this);

	Super::HandleOnStartTransition();
}

void UQuickFloatTween::HandleOnCompleteTransition(bool bSnapToEnd)
{
	Super::HandleOnCompleteTransition(bSnapToEnd);

	if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
	{
		if (Setter.IsBound())
		{
			Setter.Execute(StartValue.Get(0.0f), this);
		}
		CurrentValue = StartValue.Get(0.0f);
		return;
	}

	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickFloatTween::HandleOnCompleteTransition: 'To' delegate is not bound, unable to complete tween."));
		return;
	}

	const float value = bSnapToEnd ? To.Execute(this) : StartValue.Get(0.0f);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;
}
