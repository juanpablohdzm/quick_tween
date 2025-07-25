// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderObject.h"

#include "Tweens/QuickTweenBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenBuilder, Log, All);

FQuickTweenBuilderObject& FQuickTweenBuilderObject::SetLoops(int32 Loops)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetLoops(Loops);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return *this;
}

FQuickTweenBuilderObject& FQuickTweenBuilderObject::SetLoopType(ELoopType LoopType)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetLoopType(LoopType);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return *this;
}

FQuickTweenBuilderObject& FQuickTweenBuilderObject::SetEaseType(EEaseType EaseType)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetEaseType(EaseType);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return *this;
}

FQuickTweenBuilderObject& FQuickTweenBuilderObject::SetEaseCurve(UCurveFloat* EaseCurve)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetEaseCurve(EaseCurve);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return *this;
}

FQuickTweenBuilderObject& FQuickTweenBuilderObject::SetTimeScale(float TimeScale)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetTimeScale(TimeScale);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return *this;
}

FQuickTweenBuilderObject& FQuickTweenBuilderObject::SetIsBackwards(bool bIsBackwards)
{
	if (UQuickTweenBase* LastTween = Tweens.Last())
	{
		LastTween->SetIsBackwards(bIsBackwards);
	}
	else
	{
		UE_LOG(LogQuickTweenBuilder, Warning, TEXT("No tween to set loops on."));
	}
	return *this;
}
