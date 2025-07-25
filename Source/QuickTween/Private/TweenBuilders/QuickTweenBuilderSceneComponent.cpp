// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderSceneComponent.h"


FQuickTweenBuilderSceneComponent& FQuickTweenBuilderSceneComponent::MoveTo(FVector To, float Duration,
	FString&& TweenTag)
{
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderSceneComponent::RotateTo(FRotator To, float Duration,
                                                                             bool bUseShortestPath, FString&& TweenTag)
{
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderSceneComponent::RotateTo(FQuat To, float Duration,
	bool bUseShortestPath, FString&& TweenTag)
{
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderSceneComponent::ScaleTo(FVector To, float Duration,
	FString&& TweenTag)
{
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderSceneComponent::LookAt(FVector Forward, float Duration,
	bool bUseShortestPath, FVector Up, FString&& TweenTag)
{
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderSceneComponent::SetLoops(int32 Loops)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetLoops(Loops));
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderSceneComponent::SetLoopType(ELoopType LoopType)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetLoopType(LoopType));
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderSceneComponent::SetEaseType(EEaseType EaseType)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetEaseType(EaseType));
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderSceneComponent::SetEaseCurve(UCurveFloat* EaseCurve)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetEaseCurve(EaseCurve));
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderSceneComponent::SetTimeScale(float TimeScale)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetTimeScale(TimeScale));
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderSceneComponent::SetIsBackwards(bool bIsBackwards)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetIsBackwards(bIsBackwards));
}

UQuickTweenBase* FQuickTweenBuilderSceneComponent::Build()
{
	return FQuickTweenBuilderObject::Build();
}
