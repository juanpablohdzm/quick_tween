// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderCameraComponent.h"


UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::AspectRatioTo(float To, float Duration,
	const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::FarClipPlaneTo(float To, float Duration,
	const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::NearClipPlaneTo(float To, float Duration,
	const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::MoveTo(FVector To, float Duration,
	const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::RotateTo(FRotator To, float Duration,
	bool bUseShortestPath, const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::RotateToQuat(FQuat To, float Duration,
	bool bUseShortestPath, const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::LookAt(FVector Forward, float Duration,
	bool bUseShortestPath, FVector Up, const FString& TweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetLoops(int32 Loops)
{
	UQuickTweenBuilderObject::SetLoopsBase(Loops);
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetLoopType(ELoopType LoopType)
{
	UQuickTweenBuilderObject::SetLoopTypeBase(LoopType);
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetEaseType(EEaseType EaseType)
{
	UQuickTweenBuilderObject::SetEaseTypeBase(EaseType);
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetEaseCurve(UCurveFloat* EaseCurve)
{
	UQuickTweenBuilderObject::SetEaseCurveBase(EaseCurve);
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetTimeScale(float TimeScale)
{
	UQuickTweenBuilderObject::SetTimeScaleBase(TimeScale);
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetIsBackwards(bool bIsBackwards)
{
	UQuickTweenBuilderObject::SetIsBackwardsBase(bIsBackwards);
	return this;
}