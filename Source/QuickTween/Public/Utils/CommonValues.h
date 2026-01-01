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
	Idle = 0,
	Play = 1 << 0,
	Pause = 1 << 1,
	Complete = 1 << 2,
	Kill = 1 << 3,
};

// Valid state transitions for QuickTweenState
static TMap<EQuickTweenState, TArray<EQuickTweenState>> ValidTransitions =
{
	{EQuickTweenState::Idle, {EQuickTweenState::Play, EQuickTweenState::Kill}},
	{EQuickTweenState::Play,    {EQuickTweenState::Pause, EQuickTweenState::Complete, EQuickTweenState::Kill, EQuickTweenState::Idle}},
	{EQuickTweenState::Pause,     {EQuickTweenState::Play, EQuickTweenState::Complete, EQuickTweenState::Kill, EQuickTweenState::Idle}},
	{EQuickTweenState::Complete,  {EQuickTweenState::Idle, EQuickTweenState::Kill}},
	{EQuickTweenState::Kill,     {}},
};

// Payload structure for evaluating QuickTweens
struct FQuickTweenEvaluatePayload
{
	bool bIsActive;
	bool bIsReversed;
	bool bShouldTriggerEvents;
	float Value;
};


#define INFINITE_LOOPS -1