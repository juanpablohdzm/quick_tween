// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickRotatorTween.generated.h"

DECLARE_DELEGATE_RetVal( FRotator, FNativeRotatorGetter );
DECLARE_DELEGATE_OneParam( FNativeRotatorSetter, const FRotator& );

/**
 * Tween class for interpolating between two FRotator values over time.
 * Inherits from UQuickTweenBase and provides rotator-specific tweening functionality.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickRotatorTween : public UQuickTweenBase
{
	GENERATED_BODY()
public:

	/**
	 * Set up the rotator tween with the specified parameters.
	 *
	 * Note: The start value will be cached from the component's current location at the first update.
	 *
	 * @param from Function to get the FROM value.
	 * @param to Function to get the TO value.
	 * @param bUseShortestPath Whether to use the shortest path for interpolation.
	 * @param setterFunction Function to apply the interpolated value.
	 * @param duration Duration of the tween in seconds.
	 * @param timeScale Multiplier for the tween's speed.
	 * @param easeType Type of easing to apply.
	 * @param easeCurve Optional custom curve for easing.
	 * @param loops Number of times to loop the tween.
	 * @param loopType Type of looping behavior.
	 * @param tweenTag Optional tag for identifying the tween.
	 * @param worldContextObject Context object for world access.
	 * @param bShouldAutoKill Whether to auto-kill the tween on completion.
	 * @param bShouldPlayWhilePaused Whether the tween should play while the game is paused.
	 * @param bShouldAutoPlay Whether to start playing the tween immediately after setup.
	 */
	void SetUp(
		FNativeRotatorGetter from,
		FNativeRotatorGetter to,
		bool bUseShortestPath,
		FNativeRotatorSetter setterFunction,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = FString(),
		const UObject* worldContextObject = nullptr,
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false)
	{
		From = MoveTemp(from);
		To = MoveTemp(to);
		SetterFunction = MoveTemp(setterFunction);
		bShortestPath = bUseShortestPath;
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
			bShouldPlayWhilePaused,
			bShouldAutoPlay);

	}

	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr) override;

	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true) override;

	/** Get the current interpolated FRotator value. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FRotator GetCurrentValue() const { return CurrentValue; }

	/** Get the starting FRotator value. Set after the first tick */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FRotator GetStartValue() const { return StartValue.Get(FRotator::ZeroRotator); }
private:
	/** Starting value or function returning FRotator. */
	FNativeRotatorGetter From;

	/** Target FRotator function. */
	FNativeRotatorGetter To;

	/** Starting value. */
	TOptional<FRotator> StartValue;

	/** Function to set the interpolated FRotator value. */
	FNativeRotatorSetter SetterFunction;

	/** Whether to use the shortest path for interpolation. */
	bool bShortestPath = true;

	/** Current interpolated value. */
	FRotator CurrentValue;
};