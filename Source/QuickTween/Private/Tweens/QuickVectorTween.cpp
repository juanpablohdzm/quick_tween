// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickVectorTween.h"

#include "Utils/EaseFunctions.h"

void UQuickVectorTween::Update(float deltaTime)
{
	ElapsedTime += deltaTime * GetTimeScale();

	if (ElapsedTime >= GetDuration())
	{
		SetterFunction(To);
		SetIsCompleted(true);
	}
	else
	{
		float progress = ElapsedTime / GetDuration();
		if (UCurveFloat* curve = GetEaseCurve())
		{
			progress = curve->GetFloatValue(progress);
		}
		const FVector value = FEaseFunctions<FVector>::Ease(From, To, progress, GetEaseType());
		SetterFunction(value);
	}
}
