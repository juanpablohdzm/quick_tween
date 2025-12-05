// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickColorTween.h"
#include "Curves/CurveFloat.h"
#include "Utils/EaseFunctions.h"


void UQuickColorTween::Update(float deltaTime, UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (!StartValue.IsSet())
	{
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

	const FColor startValue = StartValue.GetValue();
	const FColor endValue = To.Execute(this);
	FColor value;
	value.R = FEaseFunctions<uint8>::Ease(startValue.R, endValue.R, progress, GetEaseType());
	value.G = FEaseFunctions<uint8>::Ease(startValue.G, endValue.G, progress, GetEaseType());
	value.B = FEaseFunctions<uint8>::Ease(startValue.B, endValue.B, progress, GetEaseType());
	value.A = FEaseFunctions<uint8>::Ease(startValue.A, endValue.A, progress, GetEaseType());

	SetterFunction.Execute(value, this);
	CurrentValue = value;
	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}
}

void UQuickColorTween::Complete(UQuickTweenable* instigator, bool bSnapToEnd)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
	{
		SetterFunction.Execute(StartValue.GetValue(), this);
		return Super::Complete(instigator, false);
	}

	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	FColor value = bSnapToEnd ? To.Execute(this) : StartValue.GetValue();
	SetterFunction.Execute(value, this);
	CurrentValue = value;
	return Super::Complete(instigator, bSnapToEnd);
}
