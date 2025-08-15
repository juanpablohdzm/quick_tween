// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderSceneComponent.h"

#include "TweenBuilders/QuickTweenBuilderObject.h"


UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::MoveTo(FVector to, float duration,
                                                                           const FString& tweenTag)
{
	if (bIsAppend)
	{

	}
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::RotateTo(FRotator to, float duration,
                                                                             bool bUseShortestPath, const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::RotateToQuat(FQuat to, float duration,
	bool bUseShortestPath, const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::ScaleTo(FVector to, float duration,
	const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::LookAt(FVector forward, float duration,
	bool bUseShortestPath, FVector up, const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetLoops(int32 loops)
{
	UQuickTweenBuilderObject::SetLoopsBase(loops);
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetLoopType(ELoopType loopType)
{
	UQuickTweenBuilderObject::SetLoopTypeBase(loopType);
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetEaseType(EEaseType easeType)
{
	UQuickTweenBuilderObject::SetEaseTypeBase(easeType);
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetEaseCurve(UCurveFloat* easeCurve)
{
	UQuickTweenBuilderObject::SetEaseCurveBase(easeCurve);
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetTimeScale(float timeScale)
{
	UQuickTweenBuilderObject::SetTimeScaleBase(timeScale);
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::SetIsBackwards(bool bIsBackwards)
{
	UQuickTweenBuilderObject::SetIsBackwardsBase(bIsBackwards);
	return this;
}
