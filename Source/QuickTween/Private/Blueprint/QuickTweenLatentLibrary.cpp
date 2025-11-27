// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Blueprint/QuickTweenLatentLibrary.h"

#include "Blueprint/QuickTweenLibrary.h"
#include "Tweens/QuickRotatorTween.h"
#include "Components/SceneComponent.h"
#include "Tweens/QuickFloatTween.h"
#include "Tweens/QuickTweenSequence.h"
#include "Tweens/QuickVectorTween.h"
#include "Engine.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenLatentLibrary, Log, All);


UQuickTweenSequence* UQuickTweenLatentLibrary::QuickTweenCreateLatentSequence(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
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
			if (bShouldAutoPlay)
			{
				sequence->Play();
			}
			return sequence;
		}
    }
	return nullptr;
}

UQuickVectorTween* UQuickTweenLatentLibrary::QuickTweenCreateLatentTweenVector(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
	FVector from,
	FVector to,
	FVectorSetter setterFunction,
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
	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickVectorTween* tween = UQuickTweenLibrary::QuickTweenCreateTweenVector(
				worldContextObject,
				from,
				to,
				setterFunction,
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
	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickRotatorTween* tween = UQuickTweenLibrary::QuickTweenCreateTweenRotator(
				worldContextObject,
				from,
				to,
				setterFunction,
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

UQuickFloatTween* UQuickTweenLatentLibrary::QuickTweenCreateLatentTweenFloat(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickFloatTween* tween = UQuickTweenLibrary::QuickTweenCreateTweenFloat(
				worldContextObject,
				from,
				to,
				setterFunction,
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

UQuickRotatorTween* UQuickTweenLatentLibrary::QuickTweenLatentLookAt_SceneComponent(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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

UQuickFloatTween* UQuickTweenLatentLibrary::QuickTweenLatentRotateAround_SceneComponent(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
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
	if (UWorld* world = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
	{
		FLatentActionManager& latentActionManager = world->GetLatentActionManager();

		if (latentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickFloatTween* tween = UQuickTweenLibrary::QuickTweenRotateAround_SceneComponent(
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
