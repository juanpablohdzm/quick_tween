// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderSceneComponent.h"

#include "TweenBuilders/QuickTweenBuilderObject.h"
#include "Tweens/QuickTweenSequence.h"
#include "Tweens/QuickVectorTween.h"

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::MoveTo(
	FVector to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve)
{
	USceneComponent* sceneComp = Cast<USceneComponent>(Target);
	UQuickVectorTween* tween = NewObject<UQuickVectorTween>();
	tween->SetUp(
		sceneComp->GetRelativeLocation(),
		to,
		[sceneComp](const FVector& v) { sceneComp->SetRelativeLocation(v); },
		duration,
		timeScale,
		easeType,
		easeCurve
	);

	if (bIsAppend)
	{
		Sequence->Append(tween);
	}
	else
	{
		Sequence->Join(tween);
	}
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::RotateTo(FRotator to, bool bUseShortestPath,
	float duration, float timeScale, EEaseType easeType, UCurveFloat* easeCurve)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::RotateToQuat(FQuat to, bool bUseShortestPath,
	float duration, float timeScale, EEaseType easeType, UCurveFloat* easeCurve)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::ScaleTo(
	FVector to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve)
{
	USceneComponent* sceneComp = Cast<USceneComponent>(Target);
	UQuickVectorTween* tween = NewObject<UQuickVectorTween>();
	tween->SetUp(
		sceneComp->GetRelativeScale3D(),
		to,
		[sceneComp](const FVector& v) { sceneComp->SetRelativeScale3D(v); },
		duration,
		timeScale,
		easeType,
		easeCurve
	);

	if (bIsAppend)
	{
		Sequence->Append(tween);
	}
	else
	{
		Sequence->Join(tween);
	}
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::LookAt(FVector forward, bool bUseShortestPath,
	FVector up, float duration, float timeScale, EEaseType easeType, UCurveFloat* easeCurve)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::Append()
{
	UQuickTweenBuilderObject::AppendBase();
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::Join()
{
	UQuickTweenBuilderObject::JoinBase();
	return this;
}
