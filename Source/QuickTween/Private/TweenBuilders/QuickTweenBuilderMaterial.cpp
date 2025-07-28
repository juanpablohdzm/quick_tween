// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderMaterial.h"


UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::VectorParameterTo(FName ParameterName, FVector To,
                                                                          float Duration, const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::ScalarParameterTo(FName ParameterName, float To, float Duration,
	const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::ColorParameterTo(FName ParameterName, FColor To, float Duration,
	const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetLoops(int32 Loops)
{
	UQuickTweenBuilderObject::SetLoopsBase(Loops);
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetLoopType(ELoopType LoopType)
{
	UQuickTweenBuilderObject::SetLoopTypeBase(LoopType);
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetEaseType(EEaseType EaseType)
{
	UQuickTweenBuilderObject::SetEaseTypeBase(EaseType);
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetEaseCurve(UCurveFloat* EaseCurve)
{
	UQuickTweenBuilderObject::SetEaseCurveBase(EaseCurve);
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetTimeScale(float TimeScale)
{
	UQuickTweenBuilderObject::SetTimeScaleBase(TimeScale);
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetIsBackwards(bool bIsBackwards)
{
	UQuickTweenBuilderObject::SetIsBackwardsBase(bIsBackwards);
	return this;
}
