// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderCameraComponent.h"


QuickTweenBuilderCameraComponent::QuickTweenBuilderCameraComponent()
{
}

QuickTweenBuilderCameraComponent::~QuickTweenBuilderCameraComponent()
{
}

FQuickTweenBuilderCameraComponent& FQuickTweenBuilderCameraComponent::AspectRatioTo(float To, float Duration,
	FString&& TweenTag)
{
}

FQuickTweenBuilderCameraComponent& FQuickTweenBuilderCameraComponent::FarClipPlaneTo(float To, float Duration,
	FString&& TweenTag)
{
}

FQuickTweenBuilderCameraComponent& FQuickTweenBuilderCameraComponent::NearClipPlaneTo(float To, float Duration,
	FString&& TweenTag)
{
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderCameraComponent::MoveTo(FVector To, float Duration,
	FString&& TweenTag)
{
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderCameraComponent::RotateTo(FRotator To, float Duration,
	bool bUseShortestPath, FString&& TweenTag)
{
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderCameraComponent::RotateTo(FQuat To, float Duration,
	bool bUseShortestPath, FString&& TweenTag)
{
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderCameraComponent::LookAt(FVector Forward, float Duration,
	bool bUseShortestPath, FVector Up, FString&& TweenTag)
{
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderCameraComponent::SetLoops(int32 Loops)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetLoops(Loops));
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderCameraComponent::SetLoopType(ELoopType LoopType)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetLoopType(LoopType));
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderCameraComponent::SetEaseType(EEaseType EaseType)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetEaseType(EaseType));
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderCameraComponent::SetEaseCurve(UCurveFloat* EaseCurve)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetEaseCurve(EaseCurve));
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderCameraComponent::SetTimeScale(float TimeScale)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetTimeScale(TimeScale));
}

FQuickTweenBuilderSceneComponent& FQuickTweenBuilderCameraComponent::SetIsBackwards(bool bIsBackwards)
{
	return static_cast<FQuickTweenBuilderSceneComponent&>(FQuickTweenBuilderObject::SetIsBackwards(bIsBackwards));
}

UQuickTweenBase* FQuickTweenBuilderCameraComponent::Build()
{
	return FQuickTweenBuilderObject::Build();
}
