// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "CommonValues.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FVectorSetter, const FVector&, value);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRotatorSetter, const FRotator&, value);
DECLARE_DYNAMIC_DELEGATE_OneParam(FloatSetter, const float, value);

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