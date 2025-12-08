// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Blueprint/QuickTweenLibrary.h"

#include "QuickTweenManager.h"
#include "Tweens/QuickFloatTween.h"
#include "Tweens/QuickRotatorTween.h"
#include "Tweens/QuickTweenSequence.h"
#include "Tweens/QuickVectorTween.h"
#include "Components/SceneComponent.h"
#include "Tweens/QuickColorTween.h"
#include "Tweens/QuickIntTween.h"
#include "Tweens/QuickVector2DTween.h"

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
	const FVector& from,
	const FVector& to,
	FVectorSetter setter,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenVector: Setter function is not bound."));
		return nullptr;
	}

	return UQuickVectorTween::CreateTween(
		FNativeVectorGetter::CreateLambda([from](UQuickVectorTween*) -> FVector { return from; }),
		FNativeVectorGetter::CreateLambda([to](UQuickVectorTween*) -> FVector { return to; }),
		FNativeVectorSetter::CreateWeakLambda(setter.GetUObject(), [setter](const FVector& v, UQuickVectorTween* tween)
		{
			setter.ExecuteIfBound(v, tween);
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

UQuickRotatorTween* UQuickTweenLibrary::QuickTweenCreateTweenRotator(
	UObject* worldContextObject,
	const FRotator& from,
	const FRotator& to,
	FRotatorSetter setter,
	bool bUseShortestPath,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenRotator: Setter function is not bound."));
		return nullptr;
	}

	return UQuickRotatorTween::CreateTween(
		FNativeRotatorGetter::CreateLambda([from](UQuickRotatorTween*) -> FRotator { return from; }),
		FNativeRotatorGetter::CreateLambda([to](UQuickRotatorTween*) -> FRotator { return to; }),
		bUseShortestPath,
		FNativeRotatorSetter::CreateWeakLambda(setter.GetUObject(),[setter](const FRotator& v, UQuickRotatorTween* tween)
		{
			setter.ExecuteIfBound(v, tween);
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

UQuickFloatTween* UQuickTweenLibrary::QuickTweenCreateTweenFloat(
	UObject* worldContextObject,
	float from,
	float to,
	FFloatSetter setter,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenFloat: Setter function is not bound."));
		return nullptr;
	}

	return UQuickFloatTween::CreateTween(
		FNativeFloatGetter::CreateLambda([from](UQuickFloatTween*) -> float { return from; }),
		FNativeFloatGetter::CreateLambda([to](UQuickFloatTween*) -> float { return to; }),
		FNativeFloatSetter::CreateWeakLambda(setter.GetUObject(), [setter](const float v, UQuickFloatTween* tween)
		{
			setter.ExecuteIfBound(v, tween);
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

UQuickVector2DTween* UQuickTweenLibrary::QuickTweenCreateTweenVector2D(
	UObject* worldContextObject,
	const FVector2D& from,
	const FVector2D& to,
	FVector2DSetter setter,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenVector2D: Setter function is not bound."));
		return nullptr;
	}

	return UQuickVector2DTween::CreateTween(
		FNativeVector2DGetter::CreateLambda([from](UQuickVector2DTween*) -> FVector2D { return from; }),
		FNativeVector2DGetter::CreateLambda([to](UQuickVector2DTween*) -> FVector2D { return to; }),
		FNativeVector2DSetter::CreateWeakLambda(setter.GetUObject(), [setter](const FVector2D& v, UQuickVector2DTween* tween)
		{
			setter.ExecuteIfBound(v, tween);
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

UQuickColorTween* UQuickTweenLibrary::QuickTweenCreateTweenColor(
	UObject* worldContextObject,
	const FColor& from,
	const FColor& to,
	FColorSetter setter,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenColor: Setter function is not bound."));
		return nullptr;
	}

	return UQuickColorTween::CreateTween(
		FNativeColorGetter::CreateLambda([from](UQuickColorTween*) -> FColor { return from; }),
		FNativeColorGetter::CreateLambda([to](UQuickColorTween*) -> FColor { return to; }),
		FNativeColorSetter::CreateWeakLambda(setter.GetUObject(), [setter](const FColor& v, UQuickColorTween* tween)
		{
			setter.ExecuteIfBound(v, tween);
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

UQuickIntTween* UQuickTweenLibrary::QuickTweenCreateTweenInt(
	UObject* worldContextObject,
	int32 from,
	int32 to,
	FIntSetter setter,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenInt: Setter function is not bound."));
		return nullptr;
	}

	return UQuickIntTween::CreateTween(
		FNativeIntGetter::CreateLambda([from](UQuickIntTween*) -> int32 { return from; }),
		FNativeIntGetter::CreateLambda([to](UQuickIntTween*) -> int32 { return to; }),
		FNativeIntSetter::CreateWeakLambda(setter.GetUObject(), [setter](const int32 v, UQuickIntTween* tween)
		{
			setter.ExecuteIfBound(v, tween);
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

UQuickVectorTween* UQuickTweenLibrary::QuickTweenMoveTo_SceneComponent(
	UObject* worldContextObject,
	USceneComponent* component,
	const FVector& to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	EQuickTweenSpace space,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveTo_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

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

UQuickVectorTween* UQuickTweenLibrary::QuickTweenMoveBy_SceneComponent(
	UObject* worldContextObject,
	USceneComponent* component,
	const FVector& by,
	float duration, 
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops, 
	ELoopType loopType, 
	EQuickTweenSpace space, 
	const FString& tweenTag, 
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveBy_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	return UQuickVectorTween::CreateTween(
		FNativeVectorGetter::CreateWeakLambda(component, [component, space](UQuickVectorTween*)->FVector
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentLocation() :
				component->GetRelativeLocation();
		}),
		FNativeVectorGetter::CreateLambda([by](UQuickVectorTween* tween)->FVector{ return tween->GetStartValue() + by; }),
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
		bShouldAutoPlay);
}

UQuickVectorTween* UQuickTweenLibrary::QuickTweenScaleTo_SceneComponent(
	UObject* worldContextObject,
	USceneComponent* component,
	const FVector& to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	EQuickTweenSpace space,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleTo_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

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
			space == EQuickTweenSpace::WorldSpace ?
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
	const FRotator& to,
	bool bUseShortestPath,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	EQuickTweenSpace space,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateTo_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

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
			space == EQuickTweenSpace::WorldSpace ?
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
	const FRotator& by,
	bool bUseShortestPath,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	EQuickTweenSpace space,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateBy_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	return UQuickRotatorTween::CreateTween(
		FNativeRotatorGetter::CreateWeakLambda(component, [component, space](UQuickRotatorTween*)->FRotator
		{
			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentRotation() :
				component->GetRelativeRotation();
		}),
		FNativeRotatorGetter::CreateLambda([by, space](UQuickRotatorTween* tween)->FRotator
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
			space == EQuickTweenSpace::WorldSpace ?
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
	const FVector& to,
	bool bUseShortestPath,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenLookAt_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	return UQuickRotatorTween::CreateTween(
		FNativeRotatorGetter::CreateWeakLambda(component, [component](UQuickRotatorTween*)->FRotator { return component->GetComponentRotation(); }),
		FNativeRotatorGetter::CreateWeakLambda(component, [to, component](UQuickRotatorTween*)->FRotator
		{
			const FVector direction = (to - component->GetComponentLocation()).GetSafeNormal();
			FRotator targetRotation = direction.Rotation();
			return targetRotation;
		}),
		bUseShortestPath,
		FNativeRotatorSetter::CreateWeakLambda(component, [component](const FRotator& v, UQuickRotatorTween*) { component->SetWorldRotation(v); }),
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
	const FVector& point,
	const FVector& normal,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateAroundPoint_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

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
	if (tweenTag.IsEmpty())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("Tween tag is empty in FindTweenByTag."));
		return nullptr;
	}

	if (UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject))
	{
		return manager->FindTweenByPredicate([&tweenTag](const UQuickTweenable* tween)->bool
		{
			return tween->GetTweenTag().Equals(tweenTag, ESearchCase::CaseSensitive);
		});
	}

	UE_LOG(LogQuickTweenLibrary, Warning, TEXT("Failed to get QuickTweenManager in FindTweenByTag."));
	return nullptr;
}