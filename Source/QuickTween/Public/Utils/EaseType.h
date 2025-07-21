// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEaseType : uint8
{
	Linear UMETA(DisplayName = "Linear"),

	InSine UMETA(DisplayName = "Ease In Sine"),
	OutSine UMETA(DisplayName = "Ease Out Sine"),
	InOutSine UMETA(DisplayName = "Ease InOut Sine"),

	InQuad UMETA(DisplayName = "Ease In Quad"),
	OutQuad UMETA(DisplayName = "Ease Out Quad"),
	InOutQuad UMETA(DisplayName = "Ease InOut Quad"),

	InCubic UMETA(DisplayName = "Ease In Cubic"),
	OutCubic UMETA(DisplayName = "Ease Out Cubic"),
	InOutCubic UMETA(DisplayName = "Ease InOut Cubic"),

	InQuart UMETA(DisplayName = "Ease In Quart"),
	OutQuart UMETA(DisplayName = "Ease Out Quart"),
	InOutQuart UMETA(DisplayName = "Ease InOut Quart"),

	InQuint UMETA(DisplayName = "Ease In Quint"),
	OutQuint UMETA(DisplayName = "Ease Out Quint"),
	InOutQuint UMETA(DisplayName = "Ease InOut Quint"),

	InExpo UMETA(DisplayName = "Ease In Expo"),
	OutExpo UMETA(DisplayName = "Ease Out Expo"),
	InOutExpo UMETA(DisplayName = "Ease InOut Expo"),

	InCirc UMETA(DisplayName = "Ease In Circ"),
	OutCirc UMETA(DisplayName = "Ease Out Circ"),
	InOutCirc UMETA(DisplayName = "Ease InOut Circ"),

	InBack UMETA(DisplayName = "Ease In Back"),
	OutBack UMETA(DisplayName = "Ease Out Back"),
	InOutBack UMETA(DisplayName = "Ease InOut Back"),

	InElastic UMETA(DisplayName = "Ease In Elastic"),
	OutElastic UMETA(DisplayName = "Ease Out Elastic"),
	InOutElastic UMETA(DisplayName = "Ease InOut Elastic"),

	InBounce UMETA(DisplayName = "Ease In Bounce"),
	OutBounce UMETA(DisplayName = "Ease Out Bounce"),
	InOutBounce UMETA(DisplayName = "Ease InOut Bounce"),
};