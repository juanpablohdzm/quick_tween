// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickVectorTween.h"

#include "Utils/EaseFunctions.h"

void UQuickVectorTween::Update(float deltaTime, Badge<UQuickTweenSequence>* badge)
{
	UQuickTweenBase::Update(deltaTime, badge);

	if (GetIsCompleted() || !GetIsPlaying()) return;

	const float currentLoopElapsedTime = FMath::Fmod(ElapsedTime, GetDuration());
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
	SetterFunction(GetIsBackwards() ? From : To);
	return Super::Complete(badge);
}
