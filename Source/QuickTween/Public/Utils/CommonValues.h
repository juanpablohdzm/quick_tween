// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "QuickTweenable.h"
#include "CommonValues.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FVectorSetter, const FVector&, value, UQuickTweenable*, tween);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FRotatorSetter, const FRotator&, value, UQuickTweenable*, tween);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FFloatSetter, const float, value, UQuickTweenable*, tween);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FVector2DSetter, const FVector2D&, value, UQuickTweenable*, tween);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FColorSetter, const FColor&, value, UQuickTweenable*, tween);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FIntSetter, const int32, value, UQuickTweenable*, tween);
DECLARE_DYNAMIC_DELEGATE_OneParam(FQuickTweenableAction, UQuickTweenable*, tween);
DECLARE_DYNAMIC_DELEGATE_OneParam(FQuickConstTweenableAction,const UQuickTweenable*, tween);

/**
 *  Enum defining the space in which a QuickTween operates.
 */
UENUM(BlueprintType)
enum class EQuickTweenSpace : uint8
{
	WorldSpace UMETA(DisplayName = "World Space"),
	LocalSpace UMETA(DisplayName = "Local Space")
};

/**
 *  Enum defining the current state of a QuickTween.
 */
UENUM(BlueprintType)
enum class EQuickTweenState : uint8
{
	Idle = 1 << 0,
	Start = 1 << 1,
	Play = 1 << 2,
	Pause = 1 << 3,
	Complete = 1 << 4,
	Kill = 1 << 5,
};


#define INFINITE_LOOPS -1