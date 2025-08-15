// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderCameraComponent.h"


UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::AspectRatioTo(float to, float duration,
	const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::FarClipPlaneTo(float to, float duration,
	const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::NearClipPlaneTo(float to, float duration,
	const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::MoveTo(FVector to, float duration,
	const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::RotateTo(FRotator to, float duration,
	bool bUseShortestPath, const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::RotateToQuat(FQuat to, float duration,
	bool bUseShortestPath, const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::LookAt(FVector Forward, float duration,
	bool bUseShortestPath, FVector up, const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetLoops(int32 loops)
{
	UQuickTweenBuilderObject::SetLoopsBase(loops);
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetLoopType(ELoopType loopType)
{
	UQuickTweenBuilderObject::SetLoopTypeBase(loopType);
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetEaseType(EEaseType easeType)
{
	UQuickTweenBuilderObject::SetEaseTypeBase(easeType);
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetEaseCurve(UCurveFloat* easeCurve)
{
	UQuickTweenBuilderObject::SetEaseCurveBase(easeCurve);
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetTimeScale(float timeScale)
{
	UQuickTweenBuilderObject::SetTimeScaleBase(timeScale);
	return this;
}

UQuickTweenBuilderCameraComponent* UQuickTweenBuilderCameraComponent::SetIsBackwards(bool bIsBackwards)
{
	UQuickTweenBuilderObject::SetIsBackwardsBase(bIsBackwards);
	return this;
}