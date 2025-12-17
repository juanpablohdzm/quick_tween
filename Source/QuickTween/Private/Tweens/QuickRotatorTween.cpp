// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickRotatorTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"

void UQuickRotatorTween::ApplyAlphaValue(float alpha)
{
	EEasePath path = bShortestPath ? EEasePath::Shortest : EEasePath::Longest;
	const FRotator value = GetEaseCurve() ?
	FEaseFunctions<FRotator>::Ease(StartValue.Get(FRotator::ZeroRotator), EndValue.Get(FRotator::ZeroRotator), alpha, GetEaseCurve(), path) :
	FEaseFunctions<FRotator>::Ease(StartValue.Get(FRotator::ZeroRotator), EndValue.Get(FRotator::ZeroRotator), alpha, GetEaseType(), path);

	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}

	CurrentValue = value;
}

void UQuickRotatorTween::HandleOnStart()
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

	if (!EndValue.IsSet())
	{
		if (!To.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickRotatorTween::HandleOnStartTransition: 'To' delegate is not bound."));
			return;
		}

		EndValue = To.Execute(this);
	}

	Super::HandleOnStart();
}

void UQuickRotatorTween::HandleOnComplete()
{
	if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
	{
		if (Setter.IsBound())
		{
			Setter.Execute(StartValue.Get(FRotator::ZeroRotator), this);
		}
		CurrentValue = StartValue.Get(FRotator::ZeroRotator);
		Super::HandleOnComplete();
		return;
	}

	bool bSnapToEnd = GetSnapToEndOnComplete();
	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	FRotator value = bSnapToEnd ? EndValue.Get(FRotator::ZeroRotator) : StartValue.Get(FRotator::ZeroRotator);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;

	Super::HandleOnComplete();
}
