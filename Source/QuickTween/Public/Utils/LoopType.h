// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LoopType.generated.h"

/**
 * Enum representing the type of looping behavior for tweens.
 * - Restart: The tween restarts from the beginning after completion.
 * - PingPong: The tween reverses direction after reaching the end, creating a back-and-forth effect.
 */
UENUM(BlueprintType)
enum class ELoopType : uint8
{
	Restart UMETA(DisplayName = "Restart"),
	PingPong UMETA(DisplayName = "Ping Pong"),
};