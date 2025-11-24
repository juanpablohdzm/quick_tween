// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/QuickTweenLibrary.h"

#include "QuickTweenManager.h"
#include "Tweens/QuickFloatTween.h"
#include "Tweens/QuickRotatorTween.h"
#include "Tweens/QuickTweenSequence.h"
#include "Tweens/QuickVectorTween.h"
#include "Components/SceneComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenLibrary, Log, All);

UQuickTweenSequence* UQuickTweenLibrary::MakeQuickTweenSequence(
	UObject* worldContextObject,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{
	UQuickTweenSequence* sequence = NewObject<UQuickTweenSequence>();
	sequence->SetUp(worldContextObject, loops, loopType, tweenTag, bShouldAutoKill, bShouldPlayWhilePaused);
	return sequence;
}

UQuickVectorTween* UQuickTweenLibrary::MakeQuickTweenVector(
	UObject* worldContextObject,
	FVectorGetter from,
	FVectorGetter to,
	FVectorSetter setter,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	EQuickTweenSpace space,
	FString tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{
	UQuickVectorTween* tween = NewObject<UQuickVectorTween>();
	tween->SetUp(
		[from]() -> FVector { return from.Execute(); },
		[to]() -> FVector { return to.Execute(); },
		[setter](const FVector& v) { setter.Execute(v); },
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused
	);

	return tween;
}

UQuickRotatorTween* UQuickTweenLibrary::MakeQuickTweenRotator(
	UObject* worldContextObject,
	FRotatorGetter from,
	FRotatorGetter to,
	FRotatorSetter setterFunction,
	bool bUseShortestPath,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	EQuickTweenSpace space,
	FString tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{
	UQuickRotatorTween* tween = NewObject<UQuickRotatorTween>();
	tween->SetUp(
		[from]() -> FRotator { return from.Execute(); },
		[to]() -> FRotator { return to.Execute(); },
		bUseShortestPath,
		[setterFunction](const FRotator& v) { setterFunction.Execute(v); },
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused
	);
	return tween;
}

UQuickFloatTween* UQuickTweenLibrary::MakeQuickTweenFloat(
	UObject* worldContextObject,
	FloatGetter from,
	FloatGetter to,
	FloatSetter setterFunction,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	FString tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{
	UQuickFloatTween* tween = NewObject<UQuickFloatTween>();
	tween->SetUp(
		[from]() -> float { return from.Execute(); },
		[to]() -> float { return to.Execute(); },
		[setterFunction](const float v) { setterFunction.Execute(v); },
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused
	);
	return tween;
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
	EQuickTweenSpace space,
	FString tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{

	UQuickVectorTween* tween = NewObject<UQuickVectorTween>();
	tween->SetUp(
		[component, space]()->FVector
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentLocation() :
				component->GetRelativeLocation();
		},
		[to]()->FVector{ return to; },
		[component, space](const FVector& v)
		{
			space == EQuickTweenSpace::WorldSpace ?
				component->SetWorldLocation(v, true, nullptr, ETeleportType::None) :
				component->SetRelativeLocation(v, true, nullptr, ETeleportType::None);
		},
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused
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
	EQuickTweenSpace space,
	FString tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{
	UQuickVectorTween* tween = NewObject<UQuickVectorTween>();
	tween->SetUp(
		[component, space]()->FVector
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentScale() :
				component->GetRelativeScale3D();
		},
		[to]()->FVector { return to; },
		[component, space](const FVector& v)
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->SetWorldScale3D(v) :
				component->SetRelativeScale3D(v);
		},
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused
	);

	return tween;
}

UQuickRotatorTween* UQuickTweenLibrary::RotateTo_SceneComponent(
	UObject* worldContextObject,
	USceneComponent* component,
	FRotator to,
	bool bUseShortestPath,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	EQuickTweenSpace space,
	FString tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{
	UQuickRotatorTween* tween = NewObject<UQuickRotatorTween>();
	tween->SetUp(
		[component, space]()->FRotator
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentRotation() :
				component->GetRelativeRotation();
		},
		[to]()->FRotator { return to; },
		bUseShortestPath,
		[component, space](const FRotator& v)
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->SetWorldRotation(v) :
				component->SetRelativeRotation(v);
		},
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused
	);

	return tween;
}

UQuickRotatorTween* UQuickTweenLibrary::LookAt_SceneComponent(
	UObject* worldContextObject,
	USceneComponent* component,
	FVector to,
	bool bUseShortestPath,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	FString tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{

	UQuickRotatorTween* tween = NewObject<UQuickRotatorTween>();
	tween->SetUp(
		[component]()->FRotator { return component->GetRelativeRotation(); },
		[to, component]()->FRotator
		{
			const FVector direction = (to - component->GetComponentLocation()).GetSafeNormal();
			FRotator targetRotation = direction.Rotation();
			return targetRotation;
		},
		bUseShortestPath,
		[component](const FRotator& v) { component->SetRelativeRotation(v); },
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused
	);

	return tween;
}

UQuickFloatTween* UQuickTweenLibrary::RotateAround_SceneComponent(
	UObject* worldContextObject,
	USceneComponent* component,
	float from,
	float to,
	FVector point,
	FVector normal,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	FString tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{
	const FVector startPoint  = component->GetComponentLocation();
	const FVector dirFromPoint = startPoint - point;

	UQuickFloatTween* tween = NewObject<UQuickFloatTween>();
	tween->SetUp(
		[from]()->float
		{
			return from;
		},
		[to]()->float { return to; },
		[dirFromPoint, point, normal, component](const float v)
		{
			const FVector rotatedPosition = point + dirFromPoint.RotateAngleAxis(v, normal.GetSafeNormal());
			component->SetWorldLocation(rotatedPosition);
		},
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused
	);

	return tween;
}


UQuickTweenable* UQuickTweenLibrary::FindTweenByTag(const UObject* worldContextObject, const FString& tweenTag)
{
	ensureAlwaysMsgf(worldContextObject, TEXT("FindTweenByTag called with a null worldContextObject. This should never happen."));

	if (UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject))
	{
		return manager->FindTweenByPredicate([&tweenTag](const UQuickTweenable* tween)->bool
		{
			return tween->GetTweenTag().Compare(tweenTag) == 0;
		});
	}

	UE_LOG(LogQuickTweenLibrary, Warning, TEXT("Failed to get QuickTweenManager in FindTweenByTag."));
	return nullptr;
}



