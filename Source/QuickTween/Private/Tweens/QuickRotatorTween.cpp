// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickRotatorTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"

void UQuickRotatorTween::ApplyAlphaValue(float alpha)
{
	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickRotatorTween::Update: 'To' delegate is not bound, unable to interpolate."));
		return;
	}

	EEasePath path = bShortestPath ? EEasePath::Shortest : EEasePath::Longest;
	const FRotator value = GetEaseCurve() ?
	FEaseFunctions<FRotator>::Ease(StartValue.Get(FRotator::ZeroRotator), To.Execute(this), alpha, GetEaseCurve(), path) :
	FEaseFunctions<FRotator>::Ease(StartValue.Get(FRotator::ZeroRotator), To.Execute(this), alpha, GetEaseType(), path);

	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}

	CurrentValue = value;
}

void UQuickRotatorTween::HandleOnStartTransition()
{
	if (!StartValue.IsSet())
	{
		if (!From.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickRotatorTween::HandleOnStartTransition: 'From' delegate is not bound."));
			return;
		}

		StartValue = From.Execute(this);
	}

	Super::HandleOnStartTransition();
}

void UQuickRotatorTween::HandleOnCompleteTransition(bool bSnapToEnd)
{
	Super::HandleOnCompleteTransition(bSnapToEnd);

	if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
	{
		if (Setter.IsBound())
		{
			Setter.Execute(StartValue.Get(FRotator::ZeroRotator), this);
		}
		CurrentValue = StartValue.Get(FRotator::ZeroRotator);
		return;
	}

	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickRotatorTween::HandleOnCompleteTransition: 'To' delegate is not bound, unable to complete tween."));
		return;
	}

	FRotator value = bSnapToEnd ? To.Execute(this) : StartValue.Get(FRotator::ZeroRotator);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;
}
