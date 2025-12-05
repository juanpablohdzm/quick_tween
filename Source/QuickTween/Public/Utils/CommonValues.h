// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "QuickTweenable.h"
#include "CommonValues.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FVectorSetter, const FVector&, value, UQuickTweenable*, tween);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FRotatorSetter, const FRotator&, value, UQuickTweenable*, tween);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FloatSetter, const float, value, UQuickTweenable*, tween);

/**
 *  Enum defining the space in which a QuickTween operates.
 */
UENUM(BlueprintType)
enum class EQuickTweenSpace : uint8
{
	WorldSpace UMETA(DisplayName = "World Space"),
	LocalSpace UMETA(DisplayName = "Local Space")
};


#define INFINITE_LOOPS -1