// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderMaterial.h"


FQuickTweenBuilderMaterial& FQuickTweenBuilderMaterial::VectorParameterTo(FName ParameterName, FVector To,
	float Duration, FString&& TweenTag)
{
}

FQuickTweenBuilderMaterial& FQuickTweenBuilderMaterial::ScalarParameterTo(FName ParameterName, float To, float Duration,
	FString&& TweenTag)
{
}

FQuickTweenBuilderMaterial& FQuickTweenBuilderMaterial::ColorParameterTo(FName ParameterName, FColor To, float Duration,
	FString&& TweenTag)
{
}

FQuickTweenBuilderMaterial& FQuickTweenBuilderMaterial::SetLoops(int32 Loops)
{
	return static_cast<FQuickTweenBuilderMaterial&>(FQuickTweenBuilderObject::SetLoops(Loops));
}

FQuickTweenBuilderMaterial& FQuickTweenBuilderMaterial::SetLoopType(ELoopType LoopType)
{
	return static_cast<FQuickTweenBuilderMaterial&>(FQuickTweenBuilderObject::SetLoopType(LoopType));
}

FQuickTweenBuilderMaterial& FQuickTweenBuilderMaterial::SetEaseType(EEaseType EaseType)
{
	return static_cast<FQuickTweenBuilderMaterial&>(FQuickTweenBuilderObject::SetEaseType(EaseType));
}

FQuickTweenBuilderMaterial& FQuickTweenBuilderMaterial::SetEaseCurve(UCurveFloat* EaseCurve)
{
	return static_cast<FQuickTweenBuilderMaterial&>(FQuickTweenBuilderObject::SetEaseCurve(EaseCurve));
}

FQuickTweenBuilderMaterial& FQuickTweenBuilderMaterial::SetTimeScale(float TimeScale)
{
	return static_cast<FQuickTweenBuilderMaterial&>(FQuickTweenBuilderObject::SetTimeScale(TimeScale));
}

FQuickTweenBuilderMaterial& FQuickTweenBuilderMaterial::SetIsBackwards(bool bIsBackwards)
{
	return static_cast<FQuickTweenBuilderMaterial&>(FQuickTweenBuilderObject::SetIsBackwards(bIsBackwards));
}

UQuickTweenBase* FQuickTweenBuilderMaterial::Build()
{
	return FQuickTweenBuilderObject::Build();
}
