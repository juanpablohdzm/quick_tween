// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Tweens/QuickTweenBase.h"
#include "Utils/LoopType.h"
#include "QuickTweenLatentLibrary.generated.h"

class UQuickTweenable;
class UQuickTweenSequence;
class UQuickVectorTween;
class UQuickRotatorTween;
class UQuickFloatTween;

UENUM()
enum class EQuickTweenLatentSteps
{
	OnStart,
	OnUpdate,
	OnComplete,
	OnKilled
};

UCLASS()
class QUICKTWEEN_API UQuickTweenLatentLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Keywords = "Tween | Sequence | Create | Make", LatentInfo = "LatentInfo"), Category = "QuickTween")
	static UQuickTweenSequence* QuickTweenCreateLatentSequence(
		UObject* worldContextObject,
		FLatentActionInfo LatentInfo,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = true);
};