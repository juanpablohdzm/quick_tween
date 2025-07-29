// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickVectorTween.h"

#include "Utils/EaseFunctions.h"

bool UQuickVectorTween::Update(float deltaTime)
{

	if (!UQuickTweenBase::Update(deltaTime)) return false;

	float progress = GetIsBackwards() ? 1.0f - ElapsedTime / GetDuration() : ElapsedTime / GetDuration();
	if (UCurveFloat* curve = GetEaseCurve())
	{
		progress = curve->GetFloatValue(progress);
	}
	const FVector value = FEaseFunctions<FVector>::Ease(From, To, progress, GetEaseType());
	SetterFunction(value);
	SetProgress(progress);

	return true;
}

UQuickTweenBase* UQuickVectorTween::Complete()
{
	SetterFunction(To);
	return Super::Complete();
}
