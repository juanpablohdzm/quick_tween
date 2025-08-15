// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * EEaseType
 *
 * Enum representing various types of easing functions for tweening animations.
 * These easing types control the rate of change of a parameter over time,
 * allowing for smooth, natural, or stylized transitions.
 *
 * Each value corresponds to a specific mathematical easing function,
 * such as linear, sine, quad, cubic, quart, quint, expo, circ, back, elastic, and bounce,
 * with variants for ease-in, ease-out, and ease-in-out.
 *
 * This enum is exposed to Blueprints.
 */
UENUM(BlueprintType)
enum class EEaseType : uint8
{
	/** Linear interpolation (no easing) */
	Linear UMETA(DisplayName = "Linear"),

	/** Easing in with a sine function */
	InSine UMETA(DisplayName = "Ease In Sine"),
	/** Easing out with a sine function */
	OutSine UMETA(DisplayName = "Ease Out Sine"),
	/** Easing in and out with a sine function */
	InOutSine UMETA(DisplayName = "Ease InOut Sine"),

	/** Easing in with a quadratic function */
	InQuad UMETA(DisplayName = "Ease In Quad"),
	/** Easing out with a quadratic function */
	OutQuad UMETA(DisplayName = "Ease Out Quad"),
	/** Easing in and out with a quadratic function */
	InOutQuad UMETA(DisplayName = "Ease InOut Quad"),

	/** Easing in with a cubic function */
	InCubic UMETA(DisplayName = "Ease In Cubic"),
	/** Easing out with a cubic function */
	OutCubic UMETA(DisplayName = "Ease Out Cubic"),
	/** Easing in and out with a cubic function */
	InOutCubic UMETA(DisplayName = "Ease InOut Cubic"),

	/** Easing in with a quartic function */
	InQuart UMETA(DisplayName = "Ease In Quart"),
	/** Easing out with a quartic function */
	OutQuart UMETA(DisplayName = "Ease Out Quart"),
	/** Easing in and out with a quartic function */
	InOutQuart UMETA(DisplayName = "Ease InOut Quart"),

	/** Easing in with a quintic function */
	InQuint UMETA(DisplayName = "Ease In Quint"),
	/** Easing out with a quintic function */
	OutQuint UMETA(DisplayName = "Ease Out Quint"),
	/** Easing in and out with a quintic function */
	InOutQuint UMETA(DisplayName = "Ease InOut Quint"),

	/** Easing in with an exponential function */
	InExpo UMETA(DisplayName = "Ease In Expo"),
	/** Easing out with an exponential function */
	OutExpo UMETA(DisplayName = "Ease Out Expo"),
	/** Easing in and out with an exponential function */
	InOutExpo UMETA(DisplayName = "Ease InOut Expo"),

	/** Easing in with a circular function */
	InCirc UMETA(DisplayName = "Ease In Circ"),
	/** Easing out with a circular function */
	OutCirc UMETA(DisplayName = "Ease Out Circ"),
	/** Easing in and out with a circular function */
	InOutCirc UMETA(DisplayName = "Ease InOut Circ"),

	/** Easing in with a back function (overshoots slightly) */
	InBack UMETA(DisplayName = "Ease In Back"),
	/** Easing out with a back function (overshoots slightly) */
	OutBack UMETA(DisplayName = "Ease Out Back"),
	/** Easing in and out with a back function (overshoots slightly) */
	InOutBack UMETA(DisplayName = "Ease InOut Back"),

	/** Easing in with an elastic function (bounces at the start) */
	InElastic UMETA(DisplayName = "Ease In Elastic"),
	/** Easing out with an elastic function (bounces at the end) */
	OutElastic UMETA(DisplayName = "Ease Out Elastic"),
	/** Easing in and out with an elastic function (bounces at both ends) */
	InOutElastic UMETA(DisplayName = "Ease InOut Elastic"),

	/** Easing in with a bounce function */
	InBounce UMETA(DisplayName = "Ease In Bounce"),
	/** Easing out with a bounce function */
	OutBounce UMETA(DisplayName = "Ease Out Bounce"),
	/** Easing in and out with a bounce function */
	InOutBounce UMETA(DisplayName = "Ease InOut Bounce"),
};