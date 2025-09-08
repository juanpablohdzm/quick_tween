// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickVectorTween.h"

#include "Utils/EaseFunctions.h"

void UQuickVectorTween::Update(float deltaTime, Badge<UQuickTweenSequence>* badge)
{
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
	const FVector value = FEaseFunctions<FVector>::Ease(From, To, progress, GetEaseType());
	SetterFunction(value);
	SetProgress(progress);
}

UQuickTweenBase* UQuickVectorTween::Complete(Badge<UQuickTweenSequence>* badge)
{
	if (GetLoopType() == ELoopType::PingPong )
	{
		const bool isOddLoop = GetCurrentLoop() % 2 == 1;
		const bool toEnd = (isOddLoop && !GetIsReversed()) || (!isOddLoop && GetIsReversed());
		SetterFunction(toEnd ? To : From);
	}
	else
	{
		SetterFunction(GetIsReversed() ? From : To);
	}
	return Super::Complete(badge);
}
