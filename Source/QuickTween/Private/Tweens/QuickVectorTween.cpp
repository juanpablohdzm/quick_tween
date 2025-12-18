// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickVectorTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"


void UQuickVectorTween::ApplyAlphaValue(float alpha)
{
	const FVector value = GetEaseCurve() ?
	FEaseFunctions<FVector>::Ease(StartValue.Get(FVector::ZeroVector), EndValue.Get(FVector::ZeroVector), alpha, GetEaseCurve()) :
	FEaseFunctions<FVector>::Ease(StartValue.Get(FVector::ZeroVector), EndValue.Get(FVector::ZeroVector), alpha, GetEaseType());
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;
}

void UQuickVectorTween::HandleOnStart()
{
	if (!StartValue.IsSet())
	{
		if (!From.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickVectorTween::HandleOnStartTransition: 'From' delegate is not bound."));
			return;
		}
		StartValue = From.Execute(this);
	}

	if (!EndValue.IsSet())
	{
		if (!To.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickVectorTween::HandleOnStartTransition: 'To' delegate is not bound."));
			return;
		}
		EndValue = To.Execute(this);
	}

	Super::HandleOnStart();
}

void UQuickVectorTween::HandleOnComplete()
{
	bool bSnapToEnd = GetSnapToEndOnComplete();
	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	bool bSnapToBeginning = !bSnapToEnd || (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0);
	const FVector value = bSnapToBeginning ? StartValue.Get(FVector::ZeroVector) : EndValue.Get(FVector::ZeroVector);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;

	Super::HandleOnComplete();
}
