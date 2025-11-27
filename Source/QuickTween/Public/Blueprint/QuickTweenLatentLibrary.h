// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Latent/FQuickTweenLatentAction.h"
#include "Tweens/QuickTweenBase.h"
#include "Utils/CommonValues.h"
#include "Utils/LoopType.h"
#include "QuickTweenLatentLibrary.generated.h"

class UQuickTweenable;
class UQuickTweenSequence;
class UQuickVectorTween;
class UQuickRotatorTween;
class UQuickFloatTween;

UCLASS()
class QUICKTWEEN_API UQuickTweenLatentLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,
		meta = (
			WorldContext = "worldContextObject",
			Latent,
			LatentInfo = "latentInfo",
			ExpandEnumAsExecs = "latentStep",
			Keywords = "Tween | Sequence | Create | Make | Latent",
			HidePin = "latentStep"
			),
		Category = "QuickTween")
	static UQuickTweenSequence* QuickTweenCreateLatentSequence(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	UFUNCTION(BlueprintCallable,
		meta = (
			WorldContext = "worldContextObject",
			Latent,
			LatentInfo = "latentInfo",
			ExpandEnumAsExecs = "latentStep",
			Keywords = "Tween | Vector | Create | Make | Latent",
			HidePin = "latentStep"
			),
		Category = "QuickTween")
	static UQuickVectorTween* QuickTweenCreateLatentTweenVector(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		FVector from,
		FVector to,
		FVectorSetter setterFunction,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);


	UFUNCTION(BlueprintCallable,
		meta = (
			WorldContext = "worldContextObject",
			Latent,
			LatentInfo = "latentInfo",
			ExpandEnumAsExecs = "latentStep",
			Keywords = "Tween | Rotator | Create | Make | Latent",
			HidePin = "latentStep"
			),
		Category = "QuickTween")
	static UQuickRotatorTween* QuickTweenCreateLatentTweenRotator(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		FRotator from,
		FRotator to,
		FRotatorSetter setterFunction,
		bool bUseShortestPath,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	UFUNCTION(BlueprintCallable,
		meta = (
			WorldContext = "worldContextObject",
			Latent,
			LatentInfo = "latentInfo",
			ExpandEnumAsExecs = "latentStep",
			Keywords = "Tween | Rotator | Create | Make | Latent",
			HidePin = "latentStep"
			),
		Category = "QuickTween")
	static UQuickFloatTween* QuickTweenCreateLatentTweenFloat(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		float from,
		float to,
		FloatSetter setterFunction,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);


	UFUNCTION(BlueprintCallable,
		meta = (
			WorldContext = "worldContextObject",
			Latent,
			LatentInfo = "latentInfo",
			ExpandEnumAsExecs = "latentStep",
			Keywords = "Tween | Vector | Create | Make | Latent",
			HidePin = "latentStep"
			),
		Category = "QuickTween")
static UQuickVectorTween* QuickTweenLatentMoveTo_SceneComponent(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		USceneComponent* component,
		FVector to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		EQuickTweenSpace space = EQuickTweenSpace::WorldSpace,
		FString tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	UFUNCTION(BlueprintCallable,
		meta = (
			WorldContext = "worldContextObject",
			Latent,
			LatentInfo = "latentInfo",
			ExpandEnumAsExecs = "latentStep",
			Keywords = "Tween | Vector | Create | Make | Latent",
			HidePin = "latentStep"
			),
		Category = "QuickTween")
	static UQuickVectorTween* QuickTweenLatentScaleTo_SceneComponent(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		USceneComponent* component,
		FVector to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		EQuickTweenSpace space = EQuickTweenSpace::LocalSpace,
		FString tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	UFUNCTION(BlueprintCallable,
		meta = (
			WorldContext = "worldContextObject",
			Latent,
			LatentInfo = "latentInfo",
			ExpandEnumAsExecs = "latentStep",
			Keywords = "Tween | Rotator | Create | Make | Latent",
			HidePin = "latentStep"
			),
		Category = "QuickTween")
		static UQuickRotatorTween* QuickTweenLatentRotateTo_SceneComponent(
        		UObject* worldContextObject,
        		FLatentActionInfo latentInfo,
				EQuickTweenLatentSteps& latentStep,
        		USceneComponent* component,
        		FRotator to,
        		bool bUseShortestPath = true,
        		float duration = 1.0f,
        		float timeScale = 1.0f,
        		EEaseType easeType = EEaseType::Linear,
        		UCurveFloat* easeCurve = nullptr,
        		int32 loops = 1,
        		ELoopType loopType = ELoopType::Restart,
        		EQuickTweenSpace space = EQuickTweenSpace::LocalSpace,
        		FString tweenTag = "",
        		bool bShouldAutoKill = false,
        		bool bShouldPlayWhilePaused = false,
        		bool bShouldAutoPlay = false);

		UFUNCTION(BlueprintCallable,
			meta = (
				WorldContext = "worldContextObject",
				Latent,
				LatentInfo = "latentInfo",
				ExpandEnumAsExecs = "latentStep",
				Keywords = "Tween | Rotator | Create | Make | Latent",
				HidePin = "latentStep"
				),
			Category = "QuickTween")
        static UQuickRotatorTween* QuickTweenLatentLookAt_SceneComponent(
        		UObject* worldContextObject,
        		FLatentActionInfo latentInfo,
				EQuickTweenLatentSteps& latentStep,
        		USceneComponent* component,
        		FVector to,
        		bool bUseShortestPath = true,
        		float duration = 1.0f,
        		float timeScale = 1.0f,
        		EEaseType easeType = EEaseType::Linear,
        		UCurveFloat* easeCurve = nullptr,
        		int32 loops = 1,
        		ELoopType loopType = ELoopType::Restart,
        		FString tweenTag = "",
        		bool bShouldAutoKill = false,
        		bool bShouldPlayWhilePaused = false,
        		bool bShouldAutoPlay = false);

		UFUNCTION(BlueprintCallable,
		meta = (
			WorldContext = "worldContextObject",
			Latent,
			LatentInfo = "latentInfo",
			ExpandEnumAsExecs = "latentStep",
			Keywords = "Tween | Float | Create | Make | Latent",
			HidePin = "latentStep"
			),
		Category = "QuickTween")
        static UQuickFloatTween* QuickTweenLatentRotateAround_SceneComponent(
        		UObject* worldContextObject,
        		FLatentActionInfo latentInfo,
				EQuickTweenLatentSteps& latentStep,
        		USceneComponent* component,
        		float from,
        		float to,
        		FVector point,
        		FVector normal,
        		float duration = 1.0f,
        		float timeScale = 1.0f,
        		EEaseType easeType = EEaseType::Linear,
        		UCurveFloat* easeCurve = nullptr,
        		int32 loops = 1,
        		ELoopType loopType = ELoopType::Restart,
        		FString tweenTag = "",
        		bool bShouldAutoKill = false,
        		bool bShouldPlayWhilePaused = false,
        		bool bShouldAutoPlay = false);
};