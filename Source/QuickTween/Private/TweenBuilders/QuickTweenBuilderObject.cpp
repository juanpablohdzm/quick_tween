// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderObject.h"

#include "Tweens/QuickTweenBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenBuilder, Log, All);

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetLoopsBase(int32 Loops)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetLoops(Loops);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetLoopTypeBase(ELoopType LoopType)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetLoopType(LoopType);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetEaseTypeBase(EEaseType EaseType)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetEaseType(EaseType);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetEaseCurveBase(UCurveFloat* EaseCurve)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetEaseCurve(EaseCurve);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetTimeScaleBase(float TimeScale)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetTimeScale(TimeScale);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetIsBackwardsBase(bool bIsBackwards)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetIsBackwards(bIsBackwards);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return this;
}
