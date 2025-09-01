// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickVectorTween.h"

#include "Utils/EaseFunctions.h"

void UQuickVectorTween::Update(float deltaTime)
{
	UQuickTweenBase::Update(deltaTime);

	if (GetIsCompleted() || !GetIsPlaying()) return;

	const float currentLoopElapsedTime = FMath::Fmod(ElapsedTime, GetDuration());
	float progress = GetIsBackwards() ? 1.0f - (currentLoopElapsedTime / GetDuration()) : currentLoopElapsedTime / GetDuration();
	if (UCurveFloat* curve = GetEaseCurve())
	{
		progress = curve->GetFloatValue(progress);
	}
	const FVector value = FEaseFunctions<FVector>::Ease(From, To, progress, GetEaseType());
	SetterFunction(value);
	SetProgress(progress);
}

UQuickTweenBase* UQuickVectorTween::Complete()
{
	bool useFrom = false;
	if (GetLoopType() == ELoopType::PingPong) {
	    useFrom = (GetLoops() % 2 == 0);
	}
	SetterFunction(useFrom ? From : To);
	return Super::Complete();
}
