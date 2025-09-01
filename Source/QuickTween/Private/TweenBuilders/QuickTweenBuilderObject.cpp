// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderObject.h"

#include "Tweens/QuickTweenBase.h"
#include "Tweens/QuickTweenSequence.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenBuilder, Log, All);

void UQuickTweenBuilderObject::Initialize(UObject* inObject)
{
	Target = inObject;
	Sequence = NewObject<UQuickTweenSequence>();
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetLoopsBase(int32 loops)
{
	Sequence->SetLoops(loops);
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetLoopTypeBase(ELoopType loopType)
{
	const int32 numTweens = Sequence->GetNumTweens();
	if (numTweens == 0)
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tweens in the sequence to set loop type on."));
		return this;
	}

	UQuickTweenBase* tween = Sequence->GetTween(numTweens - 1);
	tween->SetLoopType(loopType);
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetEaseTypeBase(EEaseType easeType)
{
	const int32 numTweens = Sequence->GetNumTweens();
	if (numTweens == 0)
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tweens in the sequence to set ease type on."));
		return this;
	}

	UQuickTweenBase* tween = Sequence->GetTween(numTweens - 1);
	tween->SetEaseType(easeType);
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetEaseCurveBase(UCurveFloat* easeCurve)
{
	const int32 numTweens = Sequence->GetNumTweens();
	if (numTweens == 0)
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tweens in the sequence to set ease curve on."));
		return this;
	}

	UQuickTweenBase* tween = Sequence->GetTween(numTweens - 1);
	tween->SetEaseCurve(easeCurve);
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetTimeScaleBase(float timeScale)
{
	const int32 numTweens = Sequence->GetNumTweens();
	if (numTweens == 0)
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tweens in the sequence to set time scale on."));
		return this;
	}

	UQuickTweenBase* tween = Sequence->GetTween(numTweens - 1);
	tween->SetTimeScale(timeScale);
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetIsBackwardsBase(bool bIsBackwards)
{
	const int32 numTweens = Sequence->GetNumTweens();
	if (numTweens == 0)
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tweens in the sequence to set is backwards on."));
		return this;
	}

	UQuickTweenBase* tween = Sequence->GetTween(numTweens - 1);
	tween->SetIsBackwards(bIsBackwards);
	return this;
}