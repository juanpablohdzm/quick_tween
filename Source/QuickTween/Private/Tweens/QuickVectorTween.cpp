// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickVectorTween.h"

#include "Utils/EaseFunctions.h"

namespace
{
	FVector GetFromValue(std::variant<FVector, TFunction<FVector()>>& from)
	{
		return std::visit([](auto&& arg)
		{
			if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, FVector>)
				return arg;
			else
				return arg();
		}, from);
	}
}

void UQuickVectorTween::Update(float deltaTime, Badge<UQuickTweenSequence>* badge)
{
	if (!StartValue.IsSet())
	{
		StartValue = GetFromValue(From);
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

	const FVector value = FEaseFunctions<FVector>::Ease(StartValue.GetValue(), To, progress, GetEaseType());
	SetterFunction(value);
	SetProgress(progress);
}

UQuickTweenBase* UQuickVectorTween::Complete(Badge<UQuickTweenSequence>* badge)
{
	if (GetLoopType() == ELoopType::PingPong )
	{
		const bool isOddLoop = GetCurrentLoop() % 2 == 1;
		const bool toEnd = (isOddLoop && !GetIsReversed()) || (!isOddLoop && GetIsReversed());
		SetterFunction(toEnd ? To : StartValue.GetValue());
	}
	else
	{
		SetterFunction(GetIsReversed() ? StartValue.GetValue() : To);
	}
	return Super::Complete(badge);
}
