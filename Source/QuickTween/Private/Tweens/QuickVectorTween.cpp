// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickVectorTween.h"

#include "Utils/EaseFunctions.h"

void UQuickVectorTween::Update(float deltaTime)
{
	float progress = GetIsBackwards() ? 1.0f - ElapsedTime / GetDuration() : ElapsedTime / GetDuration();
	if (UCurveFloat* curve = GetEaseCurve())
	{
		progress = curve->GetFloatValue(progress);
	}
	const FVector value = FEaseFunctions<FVector>::Ease(From, To, progress, GetEaseType());
	SetterFunction(value);
	SetProgress(progress);

	UQuickTweenBase::Update(deltaTime);
}
