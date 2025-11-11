// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickVectorTween.h"

#include "Utils/EaseFunctions.h"


void UQuickVectorTween::Update(float deltaTime, UQuickTweenable* instigator)
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

	const FVector value = FEaseFunctions<FVector>::Ease(StartValue.GetValue(), To(), progress, GetEaseType());
	SetterFunction(value);
	SetProgress(progress);
}

void UQuickVectorTween::Complete(UQuickTweenable* instigator, bool bSnapToEnd)
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
