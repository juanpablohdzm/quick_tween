// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickIntTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"


void UQuickIntTween::Update(float deltaTime, UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (!StartValue.IsSet())
	{
		if (!From.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickIntTween::Update: 'From' delegate is not bound."));
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
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickIntTween::Update: 'To' delegate is not bound, unable to interpolate."));
		return;
	}

	const int32 value = FEaseFunctions<int32>::Ease(StartValue.GetValue(), To.Execute(this), progress, GetEaseType());
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

void UQuickIntTween::Complete(UQuickTweenable* instigator, bool bSnapToEnd)
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
		UE_LOG(LogQuickTweenBase, Error, TEXT("UQuickIntTween::Complete: 'To' delegate is not bound, unable to complete tween."));
		return Super::Complete(instigator, bSnapToEnd);
	}

	int32 value = bSnapToEnd ? To.Execute(this) : StartValue.GetValue();
	if (Setter.IsBound())
	{
		Setter.Execute(value, this);
	}

	CurrentValue = value;
	return Super::Complete(instigator, bSnapToEnd);
}
