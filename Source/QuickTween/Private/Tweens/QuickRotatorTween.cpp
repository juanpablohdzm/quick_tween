// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickRotatorTween.h"

#include "Utils/EaseFunctions.h"

void UQuickRotatorTween::Update(float deltaTime, UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (!StartValue.IsSet())
	{
		StartValue = From();
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

	EEasePath Path = bShortestPath ? EEasePath::Shortest : EEasePath::Longest;
	const FRotator value = FEaseFunctions<FRotator>::Ease(StartValue.GetValue(), To(), progress, GetEaseType(), Path);
	SetterFunction(value);
	SetProgress(progress);
}

void UQuickRotatorTween::Complete(UQuickTweenable* instigator, bool bSnapToEnd)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
	{
		SetterFunction(StartValue.GetValue());
		return Super::Complete(instigator, false);
	}

	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}
	SetterFunction(bSnapToEnd ? To() : StartValue.GetValue());
	return Super::Complete(instigator, bSnapToEnd);
}
