// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Blueprint/QuickTweenLatentLibrary.h"

#include "Blueprint/QuickTweenLibrary.h"
#include "Tweens/QuickRotatorTween.h"
#include "Components/SceneComponent.h"
#include "Tweens/QuickFloatTween.h"
#include "Tweens/QuickTweenSequence.h"
#include "Tweens/QuickVectorTween.h"
#include "Tweens/QuickColorTween.h"
#include "Tweens/QuickIntTween.h"
#include "Tweens/QuickVector2DTween.h"
#include "Engine/World.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenLatentLibrary, Log, All);


UQuickTweenSequence* UQuickTweenLatentLibrary::QuickTweenCreateLatentSequence(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{
	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
    {
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickTweenSequence* sequence = UQuickTweenLibrary::QuickTweenCreateSequence(
				worldContextObject,
				loops,
				loopType,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, sequence, latentStep));
			return sequence;
		}
    }
	return nullptr;
}

UQuickVectorTween* UQuickTweenLatentLibrary::QuickTweenCreateLatentTweenVector(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenCreateLatentTweenVector: Setter function is not bound."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickVectorTween* tween = UQuickTweenLibrary::QuickTweenCreateTweenVector(
				worldContextObject,
				from,
				to,
				MoveTemp(setter),
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}

UQuickRotatorTween* UQuickTweenLatentLibrary::QuickTweenCreateLatentTweenRotator(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenCreateLatentTweenRotator: Setter function is not bound."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickRotatorTween* tween = UQuickTweenLibrary::QuickTweenCreateTweenRotator(
				worldContextObject,
				from,
				to,
				MoveTemp(setter),
				bUseShortestPath,
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}

UQuickVector2DTween* UQuickTweenLatentLibrary::QuickTweenCreateLatentTweenVector2D(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenCreateLatentTweenVector2D: Setter function is not bound."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickVector2DTween* tween = UQuickTweenLibrary::QuickTweenCreateTweenVector2D(
				worldContextObject,
				from,
				to,
				MoveTemp(setter),
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}

UQuickColorTween* UQuickTweenLatentLibrary::QuickTweenCreateLatentTweenColor(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenCreateLatentTweenColor: Setter function is not bound."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickColorTween* tween = UQuickTweenLibrary::QuickTweenCreateTweenColor(
				worldContextObject,
				from,
				to,
				MoveTemp(setter),
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}

UQuickIntTween* UQuickTweenLatentLibrary::QuickTweenCreateLatentTweenInt(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenCreateLatentTweenInt: Setter function is not bound."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickIntTween* tween = UQuickTweenLibrary::QuickTweenCreateTweenInt(
				worldContextObject,
				from,
				to,
				MoveTemp(setter),
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}

UQuickFloatTween* UQuickTweenLatentLibrary::QuickTweenCreateLatentTweenFloat(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenCreateLatentTweenFloat: Setter function is not bound."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickFloatTween* tween = UQuickTweenLibrary::QuickTweenCreateTweenFloat(
				worldContextObject,
				from,
				to,
				MoveTemp(setter),
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}

UQuickVectorTween* UQuickTweenLatentLibrary::QuickTweenLatentMoveTo_SceneComponent(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenLatentMoveTo_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickVectorTween* tween = UQuickTweenLibrary::QuickTweenMoveTo_SceneComponent(
				worldContextObject,
				component,
				to,
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				space,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}

UQuickVectorTween* UQuickTweenLatentLibrary::QuickTweenLatentScaleTo_SceneComponent(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenLatentScaleTo_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickVectorTween* tween = UQuickTweenLibrary::QuickTweenScaleTo_SceneComponent(
				worldContextObject,
				component,
				to,
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				space,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}

UQuickRotatorTween* UQuickTweenLatentLibrary::QuickTweenLatentRotateTo_SceneComponent(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenLatentRotateTo_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickRotatorTween* tween = UQuickTweenLibrary::QuickTweenRotateTo_SceneComponent(
				worldContextObject,
				component,
				to,
				bUseShortestPath,
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				space,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}

UQuickRotatorTween* UQuickTweenLatentLibrary::QuickTweenLatentRotateBy_SceneComponent(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenLatentRotateBy_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickRotatorTween* tween = UQuickTweenLibrary::QuickTweenRotateBy_SceneComponent(
				worldContextObject,
				component,
				by,
				bUseShortestPath,
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				space,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}

UQuickRotatorTween* UQuickTweenLatentLibrary::QuickTweenLatentLookAt_SceneComponent(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenLatentLookAt_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickRotatorTween* tween = UQuickTweenLibrary::QuickTweenLookAt_SceneComponent(
				worldContextObject,
				component,
				to,
				bUseShortestPath,
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}

UQuickFloatTween* UQuickTweenLatentLibrary::QuickTweenLatentRotateAroundPoint_SceneComponent(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
		UE_LOG(LogQuickTweenLatentLibrary, Warning, TEXT("QuickTweenLatentRotateAroundPoint_SceneComponent: SceneComponent is null."));
		return nullptr;
	}

	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickFloatTween* tween = UQuickTweenLibrary::QuickTweenRotateAroundPoint_SceneComponent(
				worldContextObject,
				component,
				from,
				to,
				point,
				normal,
				duration,
				timeScale,
				easeType,
				easeCurve,
				loops,
				loopType,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			latentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, Cast<UQuickTweenBase>(tween), latentStep));
			if (bShouldAutoPlay)
			{
				tween->Play();
			}
			return tween;
		}
	}
	return nullptr;
}
