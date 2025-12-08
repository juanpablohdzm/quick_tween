// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickRotatorTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"

void UQuickRotatorTween::Update(float deltaTime, UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (!StartValue.IsSet())
	{
		if (!From.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickRotatorTween::Update: 'From' delegate is not bound."));
			return;
		}

		StartValue = From.Execute(this);
	}

	UQuickTweenBase::Update(deltaTime, instigator);

	if (GetIsCompleted() || !GetIsPlaying()) return;

	float currentLoopElapsedTime;

	const float mod = FMath::Fmod(ElapsedTime, GetDuration());
	if (FMath::IsNearlyZero(mod))
	{
		currentLoopElapsedTime = FMath::IsNearlyZero(ElapsedTime) ? 0.0f : GetDuration();
	}
	else
	{
		currentLoopElapsedTime = mod;
	}
	float progress = FMath::Abs(currentLoopElapsedTime / GetDuration());
	if (UCurveFloat* curve = GetEaseCurve())
	{
		progress = curve->GetFloatValue(progress);
	}

	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickRotatorTween::Update: 'To' delegate is not bound, unable to interpolate."));
		return;
	}

	EEasePath Path = bShortestPath ? EEasePath::Shortest : EEasePath::Longest;
	const FRotator value = FEaseFunctions<FRotator>::Ease(StartValue.GetValue(), To.Execute(this), progress, GetEaseType(), Path);

	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}

	CurrentValue = value;
	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}
}

void UQuickRotatorTween::Complete(UQuickTweenable* instigator, bool bSnapToEnd)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
	{
		if (Setter.IsBound())
		{
			Setter.Execute(StartValue.GetValue(), this);
		}
		return Super::Complete(instigator, false);
	}

	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	if (!To.IsBound())
	{
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickRotatorTween::Complete: 'To' delegate is not bound, unable to complete tween."));
		return Super::Complete(instigator, bSnapToEnd);
	}

	FRotator value = bSnapToEnd ? To.Execute(this) : StartValue.GetValue();
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}
	CurrentValue = value;
	return Super::Complete(instigator, bSnapToEnd);
}
