// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include <variant>

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickFloatTween.generated.h"

/**
 * Tween class for interpolating between two FVector values over time.
 * Inherits from UQuickTweenBase and provides vector-specific tweening functionality.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickFloatTween : public UQuickTweenBase
{
	GENERATED_BODY()
public:

	/**
	 * Set up the vector tween with the specified parameters.
	 *
	 * @param from Function to get the FROM value.
	 * @param to Function to get the TO value.
	 * @param setterFunction Function to apply the interpolated value.
	 * @param duration Duration of the tween in seconds.
	 * @param timeScale Multiplier for the tween's speed.
	 * @param easeType Type of easing to apply.
	 * @param easeCurve Optional custom curve for easing.
	 * @param loops Number of times to loop the tween.
	 * @param loopType Type of looping behavior.
	 * @param tweenTag Optional tag for identifying the tween.
	 * @param worldContextObject Context object for world access.
	 */
	void SetUp(
		TFunction<float()>&& from,
		TFunction<float()>&& to,
		TFunction<void(const float)>&& setterFunction,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = FString(),
		const UObject* worldContextObject = nullptr,
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false)
	{
		From = from;
		To = to;
		SetterFunction = setterFunction;
		UQuickTweenBase::SetUp(
			duration,
			timeScale,
			easeType,
			easeCurve,
			loops,
			loopType,
			tweenTag,
			worldContextObject,
			bShouldAutoKill,
			bShouldPlayWhilePaused);

	}


	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr) override;

	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true) override;
private:
	/** Starting function returning float. */
	TFunction<float()> From;

	/** Target function returning float. */
	TFunction<float()> To;

	/** Starting value. */
	TOptional<float> StartValue;

	/** Function to set the interpolated FVector value. */
	TFunction<void(const float)> SetterFunction;
};