// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickVector2DTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"

void UQuickVector2DTween::ApplyAlphaValue(float alpha)
{
	const FVector2D value = GetEaseCurve() ?
	FEaseFunctions<FVector2D>::Ease(StartValue.Get(FVector2D::ZeroVector), EndValue.Get(FVector2D::ZeroVector), alpha, GetEaseCurve()) :
	FEaseFunctions<FVector2D>::Ease(StartValue.Get(FVector2D::ZeroVector), EndValue.Get(FVector2D::ZeroVector), alpha, GetEaseType());
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}

	CurrentValue = value;
}

void UQuickVector2DTween::HandleOnStart()
{

	if (!StartValue.IsSet())
	{
		if (!From.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickVector2DTween::HandleOnStartTransition: 'From' delegate is not bound."));
			return;
		}

		StartValue = From.Execute(this);
	}

	if (!EndValue.IsSet())
	{
		if (!To.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickVector2DTween::HandleOnStartTransition: 'To' delegate is not bound."));
			return;
		}

		EndValue = To.Execute(this);
	}

	Super::HandleOnStart();
}

void UQuickVector2DTween::HandleOnComplete()
{
	bool bSnapToEnd = GetSnapToEndOnComplete();
	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	bool bSnapToBeginning = !bSnapToEnd || (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0);
	const FVector2D value = bSnapToBeginning ? StartValue.Get(FVector2D::ZeroVector) : EndValue.Get(FVector2D::ZeroVector);
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;

	Super::HandleOnComplete();
}
