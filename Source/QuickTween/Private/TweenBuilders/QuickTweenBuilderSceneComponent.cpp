// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderSceneComponent.h"


UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::MoveTo(FVector To, float Duration,
                                                                           const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::RotateTo(FRotator To, float Duration,
                                                                             bool bUseShortestPath, const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::RotateToQuat(FQuat To, float Duration,
	bool bUseShortestPath, const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::ScaleTo(FVector To, float Duration,
	const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::LookAt(FVector Forward, float Duration,
	bool bUseShortestPath, FVector Up, const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetLoops(int32 Loops)
{
	UQuickTweenBuilderObject::SetLoopsBase(Loops);
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetLoopType(ELoopType LoopType)
{
	UQuickTweenBuilderObject::SetLoopTypeBase(LoopType);
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetEaseType(EEaseType EaseType)
{
	UQuickTweenBuilderObject::SetEaseTypeBase(EaseType);
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetEaseCurve(UCurveFloat* EaseCurve)
{
	UQuickTweenBuilderObject::SetEaseCurveBase(EaseCurve);
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetTimeScale(float TimeScale)
{
	UQuickTweenBuilderObject::SetTimeScaleBase(TimeScale);
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetIsBackwards(bool bIsBackwards)
{
	UQuickTweenBuilderObject::SetIsBackwardsBase(bIsBackwards);
	return this;
}