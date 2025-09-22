// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickTweenLibrary.h"

#include "TweenBuilders/QuickTweenBuilderMaterial.h"
#include "Tweens/QuickVectorTween.h"


UQuickTweenSequence* UQuickTweenLibrary::MakeQuickTweenSequence(
	UObject* worldContextObject,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill)
{
	UQuickTweenSequence* sequence = NewObject<UQuickTweenSequence>();
	sequence->SetUp(worldContextObject, loops, loopType, tweenTag, bShouldAutoKill);
	return sequence;
}

UQuickVectorTween* UQuickTweenLibrary::MoveTo_SceneComponent(
	UObject* worldContextObject,
	USceneComponent* component,
	FVector to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	FString tweenTag,
	bool bShouldAutoKill)
{

	UQuickVectorTween* tween = NewObject<UQuickVectorTween>();
	tween->SetUp(
		[component]()->FVector { return component->GetComponentLocation(); },
		to,
		[component](const FVector& v)
		{
			component->SetWorldLocation(v, true, nullptr, ETeleportType::None);
		},
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill
	);

	return tween;
}

UQuickVectorTween* UQuickTweenLibrary::ScaleTo_SceneComponent(
	UObject* worldContextObject,
	USceneComponent* component,
	FVector to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	FString tweenTag,
	bool bShouldAutoKill)
{
	UQuickVectorTween* tween = NewObject<UQuickVectorTween>();
	tween->SetUp(
		[component]()->FVector { return component->GetRelativeScale3D(); },
		to,
		[component](const FVector& v) { component->SetRelativeScale3D(v); },
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill
	);

	return tween;
}

/*UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::RotateTo(
	FRotator to,
	bool bUseShortestPath,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	FString tweenTag)
{
	return this;
}

UQuickTweenBuilderSceneComponent* UQuickTweenBuilderSceneComponent::RotateToQuat(
	FQuat to,
	bool bUseShortestPath,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	FString tweenTag)
{
	return this;
}*/

