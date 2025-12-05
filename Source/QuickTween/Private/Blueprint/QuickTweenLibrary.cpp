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
	return UQuickTweenSequence::CreateSequence(
		worldContextObject,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused);
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
	return UQuickVectorTween::CreateTween(
		FNativeVectorGetter::CreateLambda([from](UQuickVectorTween*) -> FVector { return from; }),
		FNativeVectorGetter::CreateLambda([to](UQuickVectorTween*) -> FVector { return to; }),
		FNativeVectorSetter::CreateWeakLambda(setter.GetUObject(), [setter = MoveTemp(setter)](const FVector& v, UQuickVectorTween* tween){ setter.Execute(v, tween);}),
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
	return UQuickRotatorTween::CreateTween(
		FNativeRotatorGetter::CreateLambda([from](UQuickRotatorTween*) -> FRotator { return from; }),
		FNativeRotatorGetter::CreateLambda([to](UQuickRotatorTween*) -> FRotator { return to; }),
		bUseShortestPath,
		FNativeRotatorSetter::CreateWeakLambda(setterFunction.GetUObject(),[setterFunction = MoveTemp(setterFunction)](const FRotator& v, UQuickRotatorTween* tween) { setterFunction.Execute(v, tween); }),
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
	return UQuickFloatTween::CreateTween(
		FNativeFloatGetter::CreateLambda([from](UQuickFloatTween*) -> float { return from; }),
		FNativeFloatGetter::CreateLambda([to](UQuickFloatTween*) -> float { return to; }),
		FNativeFloatSetter::CreateWeakLambda(setterFunction.GetUObject(), [setterFunction = MoveTemp(setterFunction)](const float v, UQuickFloatTween* tween) { setterFunction.Execute(v, tween); }),
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

	return UQuickVectorTween::CreateTween(
		FNativeVectorGetter::CreateWeakLambda(component, [component, space](UQuickVectorTween*)->FVector
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentLocation() :
				component->GetRelativeLocation();
		}),
		FNativeVectorGetter::CreateLambda([to](UQuickVectorTween*)->FVector{ return to; }),
		FNativeVectorSetter::CreateWeakLambda(component, [component, space](const FVector& v, UQuickVectorTween*)
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
	return UQuickVectorTween::CreateTween(
		FNativeVectorGetter::CreateWeakLambda(component,[component, space](UQuickVectorTween*)->FVector
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentScale() :
				component->GetRelativeScale3D();
		}),
		FNativeVectorGetter::CreateLambda([to](UQuickVectorTween*)->FVector { return to; }),
		FNativeVectorSetter::CreateWeakLambda(component, [component, space](const FVector& v, UQuickVectorTween*)
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
	return UQuickRotatorTween::CreateTween(
		FNativeRotatorGetter::CreateWeakLambda(component, [component, space](UQuickRotatorTween*)->FRotator
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentRotation() :
				component->GetRelativeRotation();
		}),
		FNativeRotatorGetter::CreateLambda([to](UQuickRotatorTween*)->FRotator { return to; }),
		bUseShortestPath,
		FNativeRotatorSetter::CreateWeakLambda(component, [component, space](const FRotator& v, UQuickRotatorTween*)
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
	return UQuickRotatorTween::CreateTween(
		FNativeRotatorGetter::CreateWeakLambda(component, [component, space](UQuickRotatorTween*)->FRotator
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentRotation() :
				component->GetRelativeRotation();
		}),
		FNativeRotatorGetter::CreateLambda([by, space](UQuickRotatorTween* tween)->FRotator // capture same tween that holds the lambda no need for weak ptr
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
		FNativeRotatorSetter::CreateWeakLambda(component, [component, space](const FRotator& v, UQuickRotatorTween*)
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

	return UQuickRotatorTween::CreateTween(
		FNativeRotatorGetter::CreateWeakLambda(component, [component](UQuickRotatorTween*)->FRotator { return component->GetRelativeRotation(); }),
		FNativeRotatorGetter::CreateWeakLambda(component, [to, component](UQuickRotatorTween*)->FRotator
		{
			const FVector direction = (to - component->GetComponentLocation()).GetSafeNormal();
			FRotator targetRotation = direction.Rotation();
			return targetRotation;
		}),
		bUseShortestPath,
		FNativeRotatorSetter::CreateWeakLambda(component, [component](const FRotator& v, UQuickRotatorTween*) { component->SetRelativeRotation(v); }),
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

	return UQuickFloatTween::CreateTween(
		FNativeFloatGetter::CreateLambda([from](UQuickFloatTween*)->float
		{
			return from;
		}),
		FNativeFloatGetter::CreateLambda([to](UQuickFloatTween*)->float { return to; }),
		FNativeFloatSetter::CreateWeakLambda(component, [dirFromPoint, point, normal, component](const float v, UQuickFloatTween*)
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
}


UQuickTweenable* UQuickTweenLibrary::QuickTweenFindTweenByTag(const UObject* worldContextObject, const FString& tweenTag)
{
	if (!worldContextObject)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("FindTweenByTag called with a null worldContextObject."));
		return nullptr;
	}

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



