// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Blueprint/QuickTweenLibrary.h"

#include "QuickTweenManager.h"
#include "Tweens/QuickFloatTween.h"
#include "Tweens/QuickRotatorTween.h"
#include "Tweens/QuickTweenSequence.h"
#include "Tweens/QuickVectorTween.h"
#include "Components/SceneComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenLibrary, Log, All);

UQuickTweenSequence* UQuickTweenLibrary::QuickTweenCreateSequence(
	UObject* worldContextObject,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{
	UQuickTweenSequence* sequence = NewObject<UQuickTweenSequence>();
	sequence->SetUp(
		worldContextObject,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused);
	return sequence;
}

UQuickVectorTween* UQuickTweenLibrary::QuickTweenCreateTweenVector(
	UObject* worldContextObject,
	FVector from,
	FVector to,
	FVectorSetter setter,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	FString tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	UQuickVectorTween* tween = NewObject<UQuickVectorTween>();
	tween->SetUp(
		FNativeVectorGetter::CreateLambda([from]() -> FVector { return from; }),
		FNativeVectorGetter::CreateLambda([to]() -> FVector { return to; }),
		FNativeVectorSetter::CreateWeakLambda(setter.GetUObject(), [setter = MoveTemp(setter)](const FVector& v){ setter.Execute(v);}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay
	);

	return tween;
}

UQuickRotatorTween* UQuickTweenLibrary::QuickTweenCreateTweenRotator(
	UObject* worldContextObject,
	FRotator from,
	FRotator to,
	FRotatorSetter setterFunction,
	bool bUseShortestPath,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	FString tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	UQuickRotatorTween* tween = NewObject<UQuickRotatorTween>();
	tween->SetUp(
		FNativeRotatorGetter::CreateLambda([from]() -> FRotator { return from; }),
		FNativeRotatorGetter::CreateLambda([to]() -> FRotator { return to; }),
		bUseShortestPath,
		FNativeRotatorSetter::CreateWeakLambda(setterFunction.GetUObject(),[setterFunction = MoveTemp(setterFunction)](const FRotator& v) { setterFunction.Execute(v); }),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay
	);
	return tween;
}

UQuickFloatTween* UQuickTweenLibrary::QuickTweenCreateTweenFloat(
	UObject* worldContextObject,
	float from,
	float to,
	FloatSetter setterFunction,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	FString tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	UQuickFloatTween* tween = NewObject<UQuickFloatTween>();
	tween->SetUp(
		FNativeFloatGetter::CreateLambda([from]() -> float { return from; }),
		FNativeFloatGetter::CreateLambda([to]() -> float { return to; }),
		FNativeFloatSetter::CreateWeakLambda(setterFunction.GetUObject(), [setterFunction = MoveTemp(setterFunction)](const float v) { setterFunction.Execute(v); }),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay
	);
	return tween;
}

UQuickVectorTween* UQuickTweenLibrary::QuickTweenMoveTo_SceneComponent(
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
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{

	UQuickVectorTween* tween = NewObject<UQuickVectorTween>();
	tween->SetUp(
		FNativeVectorGetter::CreateWeakLambda(component, [component, space]()->FVector
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentLocation() :
				component->GetRelativeLocation();
		}),
		FNativeVectorGetter::CreateLambda([to]()->FVector{ return to; }),
		FNativeVectorSetter::CreateWeakLambda(component, [component, space](const FVector& v)
		{
			space == EQuickTweenSpace::WorldSpace ?
				component->SetWorldLocation(v, true, nullptr, ETeleportType::None) :
				component->SetRelativeLocation(v, true, nullptr, ETeleportType::None);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay
	);

	return tween;
}

UQuickVectorTween* UQuickTweenLibrary::QuickTweenScaleTo_SceneComponent(
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
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	UQuickVectorTween* tween = NewObject<UQuickVectorTween>();
	tween->SetUp(
		FNativeVectorGetter::CreateWeakLambda(component,[component, space]()->FVector
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentScale() :
				component->GetRelativeScale3D();
		}),
		FNativeVectorGetter::CreateLambda([to]()->FVector { return to; }),
		FNativeVectorSetter::CreateWeakLambda(component, [component, space](const FVector& v)
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->SetWorldScale3D(v) :
				component->SetRelativeScale3D(v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay
	);

	return tween;
}

UQuickRotatorTween* UQuickTweenLibrary::QuickTweenRotateTo_SceneComponent(
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
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	UQuickRotatorTween* tween = NewObject<UQuickRotatorTween>();
	tween->SetUp(
		FNativeRotatorGetter::CreateWeakLambda(component, [component, space]()->FRotator
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentRotation() :
				component->GetRelativeRotation();
		}),
		FNativeRotatorGetter::CreateLambda([to]()->FRotator { return to; }),
		bUseShortestPath,
		FNativeRotatorSetter::CreateWeakLambda(component, [component, space](const FRotator& v)
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->SetWorldRotation(v) :
				component->SetRelativeRotation(v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay
	);

	return tween;
}

UQuickRotatorTween* UQuickTweenLibrary::QuickTweenRotateBy_SceneComponent(
	UObject* worldContextObject,
	USceneComponent* component,
	FRotator by,
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
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	UQuickRotatorTween* tween = NewObject<UQuickRotatorTween>();
	tween->SetUp(
		FNativeRotatorGetter::CreateWeakLambda(component, [component, space]()->FRotator
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentRotation() :
				component->GetRelativeRotation();
		}),
		FNativeRotatorGetter::CreateLambda([by, tween, space]()->FRotator // capture same tween that holds the lambda no need for weak ptr
		{
			const FQuat startRotation = tween->GetStartValue().Quaternion();
			FQuat end;
			if (space == EQuickTweenSpace::WorldSpace)
			{
				end = by.Quaternion() * startRotation;
			}
			else
			{
				end = startRotation * by.Quaternion();
			}
			return end.Rotator();
		}),
		bUseShortestPath,
		FNativeRotatorSetter::CreateWeakLambda(component, [component, space](const FRotator& v)
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->SetWorldRotation(v) :
				component->SetRelativeRotation(v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay
	);

	return tween;
}

UQuickRotatorTween* UQuickTweenLibrary::QuickTweenLookAt_SceneComponent(
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
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{

	UQuickRotatorTween* tween = NewObject<UQuickRotatorTween>();
	tween->SetUp(
		FNativeRotatorGetter::CreateWeakLambda(component, [component]()->FRotator { return component->GetRelativeRotation(); }),
		FNativeRotatorGetter::CreateWeakLambda(component, [to, component]()->FRotator
		{
			const FVector direction = (to - component->GetComponentLocation()).GetSafeNormal();
			FRotator targetRotation = direction.Rotation();
			return targetRotation;
		}),
		bUseShortestPath,
		FNativeRotatorSetter::CreateWeakLambda(component, [component](const FRotator& v) { component->SetRelativeRotation(v); }),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay
	);

	return tween;
}

UQuickFloatTween* UQuickTweenLibrary::QuickTweenRotateAroundPoint_SceneComponent(
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
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	const FVector startPoint  = component->GetComponentLocation();
	const FVector dirFromPoint = startPoint - point;

	UQuickFloatTween* tween = NewObject<UQuickFloatTween>();
	tween->SetUp(
		FNativeFloatGetter::CreateLambda([from]()->float
		{
			return from;
		}),
		FNativeFloatGetter::CreateLambda([to]()->float { return to; }),
		FNativeFloatSetter::CreateWeakLambda(component, [dirFromPoint, point, normal, component](const float v)
		{
			const FVector rotatedPosition = point + dirFromPoint.RotateAngleAxis(v, normal.GetSafeNormal());
			component->SetWorldLocation(rotatedPosition);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		worldContextObject,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay
	);

	return tween;
}


UQuickTweenable* UQuickTweenLibrary::QuickTweenFindTweenByTag(const UObject* worldContextObject, const FString& tweenTag)
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



