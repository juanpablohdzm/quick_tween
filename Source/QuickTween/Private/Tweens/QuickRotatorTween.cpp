// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickRotatorTween.h"

#include "Utils/EaseFunctions.h"

void UQuickRotatorTween::Update(float deltaTime, Badge<UQuickTweenSequence>* badge)
{
	if (!StartValue.IsSet())
	{
		StartValue = From();
	}

	UQuickTweenBase::Update(deltaTime, badge);

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

UQuickTweenBase* UQuickRotatorTween::Complete(Badge<UQuickTweenSequence>* badge)
{
	if (GetLoopType() == ELoopType::PingPong )
	{
		const bool isOddLoop = GetCurrentLoop() % 2 == 1;
		const bool toEnd = (isOddLoop && !GetIsReversed()) || (!isOddLoop && GetIsReversed());
		SetterFunction(toEnd ? To() : StartValue.GetValue());
	}
	else
	{
		SetterFunction(GetIsReversed() ? StartValue.GetValue() : To());
	}
	return Super::Complete(badge);
}
