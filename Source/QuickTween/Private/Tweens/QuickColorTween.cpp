// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickColorTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"

void UQuickColorTween::ApplyAlphaValue(float alpha)
{
	const FColor startValue = StartValue.Get(FColor::White);
	const FColor endValue = EndValue.Get(FColor::White);
	FLinearColor value =  GetEaseCurve() ?
		FEaseFunctions<FLinearColor>::Ease(FLinearColor(startValue), FLinearColor(endValue), alpha, GetEaseCurve()):
		FEaseFunctions<FLinearColor>::Ease(FLinearColor(startValue), FLinearColor(endValue), alpha, GetEaseType());

	if (Setter.IsBound())
	{
		Setter.Execute(value.ToFColor(true), this);
	}
	CurrentValue = value.ToFColor(true);
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
	bool bSnapToEnd  = GetSnapToEndOnComplete();
	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	bool bSnapToBeginning = !bSnapToEnd || (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0);
	const FColor value = bSnapToBeginning ? StartValue.Get(FColor::White) : EndValue.Get(FColor::White);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;

	Super::HandleOnComplete();
}
