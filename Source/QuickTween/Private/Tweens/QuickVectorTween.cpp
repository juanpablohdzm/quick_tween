// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickVectorTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"


void UQuickVectorTween::ApplyAlphaValue(float alpha)
{
	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickVectorTween::ApplyAlphaValue: 'To' delegate is not bound, unable to interpolate."));
		return;
	}

	const FVector value = GetEaseCurve() ?
	FEaseFunctions<FVector>::Ease(StartValue.Get(FVector::ZeroVector), To.Execute(this), alpha, GetEaseCurve()) :
	FEaseFunctions<FVector>::Ease(StartValue.Get(FVector::ZeroVector), To.Execute(this), alpha, GetEaseType());
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;
}

void UQuickVectorTween::HandleOnStartTransition()
{
	if (!From.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickVectorTween::HandleOnStartTransition: 'From' delegate is not bound."));
		return;
	}
	StartValue = From.Execute(this);

	Super::HandleOnStartTransition();
}

void UQuickVectorTween::HandleOnCompleteTransition(bool bSnapToEnd)
{
	Super::HandleOnCompleteTransition();

	if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
	{
		if (Setter.IsBound())
		{
			Setter.Execute(StartValue.Get(FVector::ZeroVector), this);
		}
		CurrentValue = StartValue.Get(FVector::ZeroVector);
		return;
	}

	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickVectorTween::HandleOnCompleteTransition: 'To' delegate is not bound, unable to interpolate."));
		return;
	}

	const FVector value = bSnapToEnd ? To.Execute(this) : StartValue.Get(FVector::ZeroVector);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;
}
