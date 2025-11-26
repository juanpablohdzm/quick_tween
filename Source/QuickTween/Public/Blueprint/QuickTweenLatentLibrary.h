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
			Keywords = "Tween | Sequence | Create | Make",
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
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = true);

	UFUNCTION(BlueprintCallable,
		meta = (
			WorldContext = "worldContextObject",
			Latent,
			LatentInfo = "latentInfo",
			ExpandEnumAsExecs = "latentStep",
			Keywords = "Tween | Vector | Create | Make",
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
		EQuickTweenSpace space = EQuickTweenSpace::WorldSpace,
		FString tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = true);


	UFUNCTION(BlueprintCallable,
		meta = (
			WorldContext = "worldContextObject",
			Latent,
			LatentInfo = "latentInfo",
			ExpandEnumAsExecs = "latentStep",
			Keywords = "Tween | Rotator | Create | Make",
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
		EQuickTweenSpace space = EQuickTweenSpace::WorldSpace,
		FString tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = true);

	UFUNCTION(BlueprintCallable,
		meta = (
			WorldContext = "worldContextObject",
			Latent,
			LatentInfo = "latentInfo",
			ExpandEnumAsExecs = "latentStep",
			Keywords = "Tween | Rotator | Create | Make",
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
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = true);
};