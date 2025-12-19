// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Blueprint/QuickTweenLibrary.h"

#include "QuickTweenManager.h"
#include "Camera/CameraComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Tweens/QuickFloatTween.h"
#include "Tweens/QuickRotatorTween.h"
#include "Tweens/QuickTweenSequence.h"
#include "Tweens/QuickVectorTween.h"
#include "Components/SceneComponent.h"
#include "Components/PanelSlot.h"
#include "Components/Widget.h"
#include "GameFramework/SpringArmComponent.h"
#include "Tweens/QuickColorTween.h"
#include "Tweens/QuickIntTween.h"
#include "Tweens/QuickVector2DTween.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenLibrary, Log, All);

UQuickTweenSequence* UQuickTweenLibrary::QuickTweenCreateSequence(
	UObject* worldContextObject,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldSnapToEndOnComplete)
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenVector: Setter function is not bound."));
		return nullptr;
	}

	return UQuickVectorTween::CreateTween(
		worldContextObject,
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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenRotator: Setter function is not bound."));
		return nullptr;
	}

	return UQuickRotatorTween::CreateTween(
		worldContextObject,
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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenFloat: Setter function is not bound."));
		return nullptr;
	}

	return UQuickFloatTween::CreateTween(
		worldContextObject,
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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenVector2D: Setter function is not bound."));
		return nullptr;
	}

	return UQuickVector2DTween::CreateTween(
		worldContextObject,
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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenColor: Setter function is not bound."));
		return nullptr;
	}

	return UQuickColorTween::CreateTween(
		worldContextObject,
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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!setter.IsBound())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCreateTweenInt: Setter function is not bound."));
		return nullptr;
	}

	return UQuickIntTween::CreateTween(
		worldContextObject,
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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveTo_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	return UQuickVectorTween::CreateTween(
		worldContextObject,
		FNativeVectorGetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), space](UQuickVectorTween*)->FVector
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveTo_SceneComponent: SceneComponent is no longer valid."));
				return FVector::ZeroVector;
			}

			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentLocation() :
				component->GetRelativeLocation();
		}),
		FNativeVectorGetter::CreateLambda([to](UQuickVectorTween*)->FVector{ return to; }),
		FNativeVectorSetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), space](const FVector& v, UQuickVectorTween*)
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveTo_SceneComponent: SceneComponent is no longer valid."));
				return;
			}

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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickVector2DTween* UQuickTweenLibrary::QuickTweenMoveTo_Widget(
	UObject* worldContextObject,
	UWidget* widget,
	const FVector2D& to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!widget)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveTo_Widget: Widget is null."));
		return nullptr;
	}

	return UQuickVector2DTween::CreateTween(
		worldContextObject,
		FNativeVector2DGetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](UQuickVector2DTween*)->FVector2D
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveTo_Widget: Widget is no longer valid."));
				return FVector2D::ZeroVector;
			}

			if (UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(widget->Slot))
			{
				return slot->GetPosition();
			}

			UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveTo_Widget: Widget is not in a CanvasPanelSlot."));
			return FVector2D::ZeroVector;
		}),
		FNativeVector2DGetter::CreateLambda([to](UQuickVector2DTween*)->FVector2D { return to; }),
		FNativeVector2DSetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](const FVector2D& v, UQuickVector2DTween*)
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveTo_Widget: Widget is no longer valid."));
				return;
			}

			if (UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(widget->Slot))
			{
				slot->SetPosition(v);
				return;
			}

			UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveTo_Widget: Widget is not in a CanvasPanelSlot."));
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveBy_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	return UQuickVectorTween::CreateTween(
		worldContextObject,
		FNativeVectorGetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), space](UQuickVectorTween*)->FVector
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveBy_SceneComponent: SceneComponent is no longer valid."));
				return FVector::ZeroVector;
			}

			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentLocation() :
				component->GetRelativeLocation();
		}),
		FNativeVectorGetter::CreateLambda([by](UQuickVectorTween* tween)->FVector{ return tween->GetStartValue() + by; }),
		FNativeVectorSetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), space](const FVector& v, UQuickVectorTween*)
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveBy_SceneComponent: SceneComponent is no longer valid."));
				return;
			}

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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete);
}

UQuickVector2DTween* UQuickTweenLibrary::QuickTweenMoveBy_Widget(
	UObject* worldContextObject,
	UWidget* widget,
	const FVector2D& by,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!widget)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveBy_Widget: Widget is null."));
		return nullptr;
	}

	return UQuickVector2DTween::CreateTween(
		worldContextObject,
		FNativeVector2DGetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](UQuickVector2DTween*)->FVector2D
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveBy_Widget: Widget is no longer valid."));
				return FVector2D::ZeroVector;
			}

			if (UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(widget->Slot))
			{
				return slot->GetPosition();
			}

			UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveBy_Widget: Widget is not in a CanvasPanelSlot."));
			return FVector2D::ZeroVector;
		}),
		FNativeVector2DGetter::CreateLambda([by](UQuickVector2DTween* tween)->FVector2D { return tween->GetStartValue() + by; }),
		FNativeVector2DSetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](const FVector2D& v, UQuickVector2DTween*)
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveBy_Widget: Widget is no longer valid."));
				return;
			}

			if (UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(widget->Slot))
			{
				slot->SetPosition(v);
				return;
			}

			UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenMoveBy_Widget: Widget is not in a CanvasPanelSlot."));
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleTo_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	return UQuickVectorTween::CreateTween(
		worldContextObject,
		FNativeVectorGetter::CreateWeakLambda(component,[component = TWeakObjectPtr(component), space](UQuickVectorTween*)->FVector
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleTo_SceneComponent: SceneComponent is no longer valid."));
				return FVector::ZeroVector;
			}

			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentScale() :
				component->GetRelativeScale3D();
		}),
		FNativeVectorGetter::CreateLambda([to](UQuickVectorTween*)->FVector { return to; }),
		FNativeVectorSetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), space](const FVector& v, UQuickVectorTween*)
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleTo_SceneComponent: SceneComponent is no longer valid."));
				return;
			}

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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickVector2DTween* UQuickTweenLibrary::QuickTweenScaleTo_Widget(
	UObject* worldContextObject,
	UWidget* widget,
	const FVector2D& to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!widget)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleTo_Widget: Widget is null."));
		return nullptr;
	}

	return UQuickVector2DTween::CreateTween(
		worldContextObject,
		FNativeVector2DGetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](UQuickVector2DTween*)->FVector2D
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleTo_Widget: Widget is no longer valid."));
				return FVector2D::ZeroVector;
			}

			return widget->GetRenderTransform().Scale;
		}),
		FNativeVector2DGetter::CreateLambda([to](UQuickVector2DTween*)->FVector2D { return to; }),
		FNativeVector2DSetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](const FVector2D& v, UQuickVector2DTween*)
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleTo_Widget: Widget is no longer valid."));
				return;
			}

			widget->SetRenderScale(v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickVectorTween* UQuickTweenLibrary::QuickTweenScaleBy_SceneComponent(
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleBy_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	return UQuickVectorTween::CreateTween(
		worldContextObject,
		FNativeVectorGetter::CreateWeakLambda(component,[component = TWeakObjectPtr(component), space](UQuickVectorTween*)->FVector
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleBy_SceneComponent: SceneComponent is no longer valid."));
				return FVector::ZeroVector;
			}

			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentScale() :
				component->GetRelativeScale3D();
		}),
		FNativeVectorGetter::CreateLambda([by](UQuickVectorTween* tween)->FVector { return tween->GetStartValue() + by; }),
		FNativeVectorSetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), space](const FVector& v, UQuickVectorTween*)
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleBy_SceneComponent: SceneComponent is no longer valid."));
				return;
			}

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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickVector2DTween* UQuickTweenLibrary::QuickTweenScaleBy_Widget(
	UObject* worldContextObject,
	UWidget* widget,
	const FVector2D& by,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!widget)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleBy_Widget: Widget is null."));
		return nullptr;
	}

	return UQuickVector2DTween::CreateTween(
		worldContextObject,
		FNativeVector2DGetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](UQuickVector2DTween*)->FVector2D
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleBy_Widget: Widget is no longer valid."));
				return FVector2D::ZeroVector;
			}

			return widget->GetRenderTransform().Scale;
		}),
		FNativeVector2DGetter::CreateLambda([by](UQuickVector2DTween* tween)->FVector2D { return tween->GetStartValue() + by; }),
		FNativeVector2DSetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](const FVector2D& v, UQuickVector2DTween*)
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScaleBy_Widget: Widget is no longer valid."));
				return;
			}

			widget->SetRenderScale(v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateTo_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	return UQuickRotatorTween::CreateTween(
		worldContextObject,
		FNativeRotatorGetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), space](UQuickRotatorTween*)->FRotator
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateTo_SceneComponent: SceneComponent is no longer valid."));
				return FRotator::ZeroRotator;
			}

			return space == EQuickTweenSpace::WorldSpace ?
				component->GetComponentRotation() :
				component->GetRelativeRotation();
		}),
		FNativeRotatorGetter::CreateLambda([to](UQuickRotatorTween*)->FRotator { return to; }),
		bUseShortestPath,
		FNativeRotatorSetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), space](const FRotator& v, UQuickRotatorTween*)
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateTo_SceneComponent: SceneComponent is no longer valid."));
				return;
			}

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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickFloatTween* UQuickTweenLibrary::QuickTweenRotateTo_Widget(
	UObject* worldContextObject,
	UWidget* widget,
	float to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!widget)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateTo_Widget: Widget is null."));
		return nullptr;
	}

	return UQuickFloatTween::CreateTween(
		worldContextObject,
		FNativeFloatGetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](UQuickFloatTween*)->float
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateTo_Widget: Widget is no longer valid."));
				return 0.0f;
			}

			return widget->GetRenderTransform().Angle;
		}),
		FNativeFloatGetter::CreateLambda([to](UQuickFloatTween*)->float { return to; }),
		FNativeFloatSetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](float v, UQuickFloatTween*)
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateTo_Widget: Widget is no longer valid."));
				return;
			}

			widget->SetRenderTransformAngle(v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateBy_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	return UQuickRotatorTween::CreateTween(
		worldContextObject,
		FNativeRotatorGetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), space](UQuickRotatorTween*)->FRotator
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateBy_SceneComponent: SceneComponent is no longer valid."));
				return FRotator::ZeroRotator;
			}

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
		FNativeRotatorSetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), space](const FRotator& v, UQuickRotatorTween*)
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateBy_SceneComponent: SceneComponent is no longer valid."));
				return;
			}

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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickFloatTween* UQuickTweenLibrary::QuickTweenRotateBy_Widget(
	UObject* worldContextObject,
	UWidget* widget,
	float by,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!widget)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateBy_Widget: Widget is null."));
		return nullptr;
	}

	return UQuickFloatTween::CreateTween(
		worldContextObject,
		FNativeFloatGetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](UQuickFloatTween*)->float
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateBy_Widget: Widget is no longer valid."));
				return 0.0f;
			}

			return widget->GetRenderTransform().Angle;
		}),
		FNativeFloatGetter::CreateLambda([by](UQuickFloatTween* tween)->float { return tween->GetStartValue() + by; }),
		FNativeFloatSetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](float v, UQuickFloatTween*)
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateBy_Widget: Widget is no longer valid."));
				return;
			}

			widget->SetRenderTransformAngle(v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenLookAt_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	return UQuickRotatorTween::CreateTween(
		worldContextObject,
		FNativeRotatorGetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component)](UQuickRotatorTween*)->FRotator
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenLookAt_SceneComponent: SceneComponent is no longer valid."));
				return FRotator::ZeroRotator;
			}

			return component->GetComponentRotation();
		}),
		FNativeRotatorGetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), to](UQuickRotatorTween*)->FRotator
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenLookAt_SceneComponent: SceneComponent is no longer valid."));
				return FRotator::ZeroRotator;
			}

			const FVector direction = (to - component->GetComponentLocation()).GetSafeNormal();
			FRotator targetRotation = direction.Rotation();
			return targetRotation;
		}),
		bUseShortestPath,
		FNativeRotatorSetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component)](const FRotator& v, UQuickRotatorTween*)
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenLookAt_SceneComponent: SceneComponent is no longer valid."));
				return;
			}

			component->SetWorldRotation(v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!component)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateAroundPoint_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	struct FStartPosHolder
	{
		bool bInitialized = false;
		FVector Start{FVector::ZeroVector};
	};

	TSharedPtr<FStartPosHolder> startPosPtr = MakeShared<FStartPosHolder>();

	return UQuickFloatTween::CreateTween(
		worldContextObject,
		FNativeFloatGetter::CreateLambda([from](UQuickFloatTween*)->float
		{
			return from;
		}),
		FNativeFloatGetter::CreateLambda([to](UQuickFloatTween*)->float
		{
			return to;
		}),
		FNativeFloatSetter::CreateWeakLambda(component, [component = TWeakObjectPtr(component), startPosPtr, point, normal](const float v, UQuickFloatTween* tween)
		{
			if (!component.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenRotateAroundPoint_SceneComponent: SceneComponent is no longer valid."));
				return;
			}

			// ... lazy initialize start position
			if (!startPosPtr->bInitialized)
			{
				startPosPtr->Start = component->GetComponentLocation();
				startPosPtr->bInitialized = true;
			}

			const FVector dirFromPoint = (startPosPtr->Start - point);
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
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickColorTween* UQuickTweenLibrary::QuickTweenChangeColorTo_Image(
	UObject* worldContextObject,
	UImage* widget,
	const FColor& to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!widget)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenColorTo_Image: Widget is null."));
		return nullptr;
	}

	return UQuickColorTween::CreateTween(
		worldContextObject,
		FNativeColorGetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](UQuickColorTween*)->FColor
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenColorTo_Widget: Widget is no longer valid."));
				return FColor::White;
			}

			return widget->GetColorAndOpacity().ToFColor(true);
		}),
		FNativeColorGetter::CreateLambda([to](UQuickColorTween*)->FColor { return to; }),
		FNativeColorSetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](const FColor& v, UQuickColorTween*)
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenColorTo_Widget: Widget is no longer valid."));
				return;
			}

			widget->SetColorAndOpacity(FLinearColor(v));
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickFloatTween* UQuickTweenLibrary::QuickTweenChangeOpacityTo_Widget(
	UObject* worldContextObject,
	UWidget* widget,
	float to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!widget)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenOpacityTo_Widget: Widget is null."));
		return nullptr;
	}

	return UQuickFloatTween::CreateTween(
		worldContextObject,
		FNativeFloatGetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](UQuickFloatTween*)->float
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenOpacityTo_Widget: Widget is no longer valid."));
				return 1.0f;
			}

			return widget->GetRenderOpacity();
		}),
		FNativeFloatGetter::CreateLambda([to](UQuickFloatTween*)->float { return to; }),
		FNativeFloatSetter::CreateWeakLambda(widget, [widget = TWeakObjectPtr(widget)](float v, UQuickFloatTween*)
		{
			if (!widget.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenOpacityTo_Widget: Widget is no longer valid."));
				return;
			}

			widget->SetRenderOpacity(v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickFloatTween* UQuickTweenLibrary::QuickTweenChangeFovTo_Camera(
	UObject* worldContextObject,
	UCameraComponent* camera,
	float to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!camera)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenToFov_Camera: CameraComponent is null."));
		return nullptr;
	}

	return UQuickFloatTween::CreateTween(
		worldContextObject,
		FNativeFloatGetter::CreateWeakLambda(camera, [camera = TWeakObjectPtr(camera)](UQuickFloatTween*)->float
		{
			if (!camera.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenToFov_Camera: CameraComponent is no longer valid."));
				return 90.0f;
			}

			return camera->FieldOfView;
		}),
		FNativeFloatGetter::CreateLambda([to](UQuickFloatTween*)->float { return to; }),
		FNativeFloatSetter::CreateWeakLambda(camera, [camera = TWeakObjectPtr(camera)](float v, UQuickFloatTween*)
		{
			if (!camera.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenToFov_Camera: CameraComponent is no longer valid."));
				return;
			}

			camera->SetFieldOfView(v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickFloatTween* UQuickTweenLibrary::QuickTweenChangeDistanceTo_SpringArm(
	UObject* worldContextObject,
	USpringArmComponent* springArm,
	float to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!springArm)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenDistanceTo_SpringArm: SpringArmComponent is null."));
		return nullptr;
	}

	return UQuickFloatTween::CreateTween(
		worldContextObject,
		FNativeFloatGetter::CreateWeakLambda(springArm, [springArm = TWeakObjectPtr(springArm)](UQuickFloatTween*)->float
		{
			if (!springArm.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenDistanceTo_SpringArm: SpringArmComponent is no longer valid."));
				return 300.0f;
			}

			return springArm->TargetArmLength;
		}),
		FNativeFloatGetter::CreateLambda([to](UQuickFloatTween*)->float { return to; }),
		FNativeFloatSetter::CreateWeakLambda(springArm, [springArm = TWeakObjectPtr(springArm)](float v, UQuickFloatTween*)
		{
			if (!springArm.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenDistanceTo_SpringArm: SpringArmComponent is no longer valid."));
				return;
			}

			springArm->TargetArmLength = v;
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickFloatTween* UQuickTweenLibrary::QuickTweenChangeDistanceBy_SpringArm(
	UObject* worldContextObject,
	USpringArmComponent* springArm,
	float by,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!springArm)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenDistanceBy_SpringArm: SpringArmComponent is null."));
		return nullptr;
	}

	return UQuickFloatTween::CreateTween(
		worldContextObject,
		FNativeFloatGetter::CreateWeakLambda(springArm, [springArm = TWeakObjectPtr(springArm)](UQuickFloatTween*)->float
		{
			if (!springArm.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenDistanceBy_SpringArm: SpringArmComponent is no longer valid."));
				return 300.0f;
			}

			return springArm->TargetArmLength;
		}),
		FNativeFloatGetter::CreateLambda([by](UQuickFloatTween* tween)->float { return tween->GetStartValue() + by; }),
		FNativeFloatSetter::CreateWeakLambda(springArm, [springArm = TWeakObjectPtr(springArm)](float v, UQuickFloatTween*)
		{
			if (!springArm.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenDistanceBy_SpringArm: SpringArmComponent is no longer valid."));
				return;
			}

			springArm->TargetArmLength = v;
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickVectorTween* UQuickTweenLibrary::QuickTweenVectorParameterTo_Material(
	UObject* worldContextObject,
	UMaterialInstanceDynamic* material,
	const FName& parameterName,
	const FVector& to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!material)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenVectorParameterTo_Material: MaterialInstanceDynamic is null."));
		return nullptr;
	}

	return UQuickVectorTween::CreateTween(
		worldContextObject,
		FNativeVectorGetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](UQuickVectorTween*)->FVector
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenVectorParameterTo_Material: MaterialInstanceDynamic is no longer valid."));
				return FVector::ZeroVector;
			}

			FLinearColor color;
			material->GetVectorParameterValue(parameterName, color);
			return FVector(color.R, color.G, color.B);
		}),
		FNativeVectorGetter::CreateLambda([to](UQuickVectorTween*)->FVector { return to; }),
		FNativeVectorSetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](const FVector& v, UQuickVectorTween*)
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenVectorParameterTo_Material: MaterialInstanceDynamic is no longer valid."));
				return;
			}

			material->SetVectorParameterValue(parameterName, FLinearColor(v.X, v.Y, v.Z));
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickVectorTween* UQuickTweenLibrary::QuickTweenVectorParameterBy_Material(
	UObject* worldContextObject,
	UMaterialInstanceDynamic* material,
	const FName& parameterName,
	const FVector& by,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!material)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenVectorParameterBy_Material: MaterialInstanceDynamic is null."));
		return nullptr;
	}

	return UQuickVectorTween::CreateTween(
		worldContextObject,
		FNativeVectorGetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](UQuickVectorTween*)->FVector
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenVectorParameterBy_Material: MaterialInstanceDynamic is no longer valid."));
				return FVector::ZeroVector;
			}

			FLinearColor color;
			material->GetVectorParameterValue(parameterName, color);
			return FVector(color.R, color.G, color.B);
		}),
		FNativeVectorGetter::CreateLambda([by](UQuickVectorTween* tween)->FVector { return tween->GetStartValue() + by; }),
		FNativeVectorSetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](const FVector& v, UQuickVectorTween*)
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenVectorParameterBy_Material: MaterialInstanceDynamic is no longer valid."));
				return;
			}

			material->SetVectorParameterValue(parameterName, FLinearColor(v.X, v.Y, v.Z));
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickFloatTween* UQuickTweenLibrary::QuickTweenScalarParameterTo_Material(
	UObject* worldContextObject,
	UMaterialInstanceDynamic* material,
	const FName& parameterName,
	float to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!material)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScalarParameterTo_Material: MaterialInstanceDynamic is null."));
		return nullptr;
	}

	return UQuickFloatTween::CreateTween(
		worldContextObject,
		FNativeFloatGetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](UQuickFloatTween*)->float
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScalarParameterTo_Material: MaterialInstanceDynamic is no longer valid."));
				return 0.0f;
			}

			float value = 0.0f;
			material->GetScalarParameterValue(parameterName, value);
			return value;
		}),
		FNativeFloatGetter::CreateLambda([to](UQuickFloatTween*)->float { return to; }),
		FNativeFloatSetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](float v, UQuickFloatTween*)
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScalarParameterTo_Material: MaterialInstanceDynamic is no longer valid."));
				return;
			}

			material->SetScalarParameterValue(parameterName, v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickFloatTween* UQuickTweenLibrary::QuickTweenScalarParameterBy_Material(
	UObject* worldContextObject,
	UMaterialInstanceDynamic* material,
	const FName& parameterName,
	float by,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!material)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScalarParameterBy_Material: MaterialInstanceDynamic is null."));
		return nullptr;
	}

	return UQuickFloatTween::CreateTween(
		worldContextObject,
		FNativeFloatGetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](UQuickFloatTween*)->float
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScalarParameterTo_Material: MaterialInstanceDynamic is no longer valid."));
				return 0.0f;
			}

			float value = 0.0f;
			material->GetScalarParameterValue(parameterName, value);
			return value;
		}),
		FNativeFloatGetter::CreateLambda([by](UQuickFloatTween* tween)->float { return  tween->GetStartValue() + by; }),
		FNativeFloatSetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](float v, UQuickFloatTween*)
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenScalarParameterTo_Material: MaterialInstanceDynamic is no longer valid."));
				return;
			}

			material->SetScalarParameterValue(parameterName, v);
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickColorTween* UQuickTweenLibrary::QuickTweenColorParameterTo_Material(
	UObject* worldContextObject,
	UMaterialInstanceDynamic* material,
	const FName& parameterName,
	const FColor& to,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!material)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenColorParameterTo_Material: MaterialInstanceDynamic is null."));
		return nullptr;
	}

	return UQuickColorTween::CreateTween(
		worldContextObject,
		FNativeColorGetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](UQuickColorTween*)->FColor
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenColorParameterTo_Material: MaterialInstanceDynamic is no longer valid."));
				return FColor::White;
			}

			FLinearColor color;
			material->GetVectorParameterValue(parameterName, color);
			return color.ToFColor(true);
		}),
		FNativeColorGetter::CreateLambda([to](UQuickColorTween*)->FColor { return to; }),
		FNativeColorSetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](const FColor& v, UQuickColorTween*)
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenColorParameterTo_Material: MaterialInstanceDynamic is no longer valid."));
				return;
			}

			material->SetVectorParameterValue(parameterName, FLinearColor(v));
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}

UQuickColorTween* UQuickTweenLibrary::QuickTweenColorParameterBy_Material(
	UObject* worldContextObject,
	UMaterialInstanceDynamic* material,
	const FName& parameterName,
	const FColor& by,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
{
	if (!material)
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenColorParameterBy_Material: MaterialInstanceDynamic is null."));
		return nullptr;
	}

	return UQuickColorTween::CreateTween(
		worldContextObject,
		FNativeColorGetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](UQuickColorTween*)->FColor
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenColorParameterBy_Material: MaterialInstanceDynamic is no longer valid."));
				return FColor::White;
			}

			FLinearColor color;
			material->GetVectorParameterValue(parameterName, color);
			return color.ToFColor(true);
		}),
		FNativeColorGetter::CreateLambda([by](UQuickColorTween* tween)->FColor
		{
			FColor start = tween->GetStartValue();
			return FColor(
				FMath::Clamp(start.R + by.R, 0.0f, 255.0f),
				FMath::Clamp(start.G + by.G, 0.0f, 255.0f),
				FMath::Clamp(start.B + by.B, 0.0f, 255.0f),
				FMath::Clamp(start.A + by.A, 0.0f, 255.0f)
			);
		}),
		FNativeColorSetter::CreateWeakLambda(material, [material = TWeakObjectPtr(material), parameterName](const FColor& v, UQuickColorTween*)
		{
			if (!material.IsValid())
			{
				UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenColorParameterBy_Material: MaterialInstanceDynamic is no longer valid."));
				return;
			}

			material->SetVectorParameterValue(parameterName, FLinearColor(v));
		}),
		duration,
		timeScale,
		easeType,
		easeCurve,
		loops,
		loopType,
		tweenTag,
		bShouldAutoKill,
		bShouldPlayWhilePaused,
		bShouldAutoPlay,
		bShouldSnapToEndOnComplete
	);
}


UQuickTweenable* UQuickTweenLibrary::QuickTweenFindTweenByTag(const UObject* worldContextObject, const FString& tweenTag)
{
	if (tweenTag.IsEmpty())
	{
		UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenFindByTag: Tween tag is empty."));
		return nullptr;
	}

	if (UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject))
	{
		return manager->FindTweenByPredicate([&tweenTag](const UQuickTweenable* tween)->bool
		{
			return tween->GetTweenTag().Equals(tweenTag, ESearchCase::CaseSensitive);
		});
	}

	UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenFindByTag: Failed to get QuickTweenManager."));
	return nullptr;
}

TArray<UQuickTweenable*> UQuickTweenLibrary::QuickTweenFindAllTweensByPredicate(
	const UObject* worldContextObject,
	const FQuickConstTweenableAction& predicate)
{
	if (UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject))
	{
		return manager->FindAllTweensByPredicate(
			[&predicate](const UQuickTweenable* tween)->bool
			{
				return predicate.ExecuteIfBound(tween);
			});
	}
	UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenFindAllByPredicate: Failed to get QuickTweenManager."));
	return TArray<UQuickTweenable*>();
}

void UQuickTweenLibrary::QuickTweenKillAllTweens(const UObject* worldContextObject)
{
	if (UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject))
	{
		manager->ExecutePredicateByCondition(
			[](UQuickTweenable* tween){ tween->Kill(); },
			[](const UQuickTweenable*){ return true; }); // ...kill all
	}
	UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenKillAllTweens: Failed to get QuickTweenManager."));
}

void UQuickTweenLibrary::QuickTweenPauseAllTweens(const UObject* worldContextObject)
{
	if (UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject))
	{
		manager->ExecutePredicateByCondition(
			[](UQuickTweenable* tween){ tween->Pause(); },
			[](const UQuickTweenable*){ return true; }); // ...pause all
	}
	UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenPauseAllTweens: Failed to get QuickTweenManager."));
}

void UQuickTweenLibrary::QuickTweenPlayAllTweens(const UObject* worldContextObject)
{
	if (UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject))
	{
		manager->ExecutePredicateByCondition(
			[](UQuickTweenable* tween){ tween->Play(); },
			[](const UQuickTweenable*){ return true; }); // ...play all
	}
	UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenPlayAllTweens: Failed to get QuickTweenManager."));
}

void UQuickTweenLibrary::QuickTweenReverseAllTweens(const UObject* worldContextObject)
{
	if (UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject))
	{
		manager->ExecutePredicateByCondition(
			[](UQuickTweenable* tween){ tween->Reverse(); },
			[](const UQuickTweenable*){ return true; }); // ...reverse all
	}
	UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenReverseAllTweens: Failed to get QuickTweenManager."));
}

void UQuickTweenLibrary::QuickTweenCompleteAllTweens(const UObject* worldContextObject)
{
	if (UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject))
	{
		manager->ExecutePredicateByCondition(
			[](UQuickTweenable* tween){ tween->Complete(); },
			[](const UQuickTweenable*){ return true; }); // ...complete all
	}
	UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenCompleteAllTweens: Failed to get QuickTweenManager."));
}

void UQuickTweenLibrary::QuickTweenExecuteActionOnAllTweens(
	const UObject* worldContextObject,
	const FQuickTweenableAction& action)
{
	if (UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject))
	{
		manager->ExecutePredicateByCondition(
			[&action](UQuickTweenable* tween){ action.ExecuteIfBound(tween); },
			[](const UQuickTweenable*){ return true; }); // ...on all
	}
	UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenExecuteActionOnAllTweens: Failed to get QuickTweenManager."));
}

void UQuickTweenLibrary::QuickTweenExecuteActionByPredicate(
	const UObject* worldContextObject,
	const FQuickConstTweenableAction& predicate, const FQuickTweenableAction& action)
{
	if (UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject))
	{
		manager->ExecutePredicateByCondition(
			[&action](UQuickTweenable* tween){ action.ExecuteIfBound(tween); },
			[&predicate](const UQuickTweenable* tween){ return predicate.ExecuteIfBound(tween); });
	}
	UE_LOG(LogQuickTweenLibrary, Warning, TEXT("QuickTweenExecuteActionByPredicate: Failed to get QuickTweenManager."));
}
