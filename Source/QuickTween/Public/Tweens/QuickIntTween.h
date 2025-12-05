// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickFloatTween.generated.h"

class UQuickIntTween;
DECLARE_DELEGATE_RetVal_OneParam(float, FNativeIntGetter, UQuickIntTween*);
DECLARE_DELEGATE_TwoParams( FNativeIntSetter, const int32, UQuickIntTween* );
/**
 * Tween class for interpolating between two int32 values over time.
 * Inherits from UQuickTweenBase and provides vector-specific tweening functionality.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickIntTween : public UQuickTweenBase
{
	GENERATED_BODY()
private:
	UQuickIntTween() = default;

	/**
	 * Set up the int tween with the specified parameters.
	 */
	void SetUp(
		FNativeIntGetter from,
		FNativeIntGetter to,
		FNativeIntSetter setterFunction,
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

public:

	/**
	 * Create and set up a UQuickIntTween with the specified parameters.
	 *
	 * Note: The start value will be cached from the component's current location at the first update.
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
	 * @param bShouldAutoKill Whether to auto-kill the tween on completion.
	 * @param bShouldPlayWhilePaused Whether the tween should play while the game is paused.
	 * @param bShouldAutoPlay Whether to start playing the tween immediately after setup.
	 */
	static UQuickIntTween* CreateTween(
		FNativeIntGetter from,
		FNativeIntGetter to,
		FNativeIntSetter setterFunction,
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
		UQuickIntTween* tween = NewObject<UQuickIntTween>();
		tween->SetUp(
			MoveTemp(from),
			MoveTemp(to),
			MoveTemp(setterFunction),
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
		return tween;
	}

	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr) override;

	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true) override;

	/** Get the current interpolated int32 value. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category= "Tween|Info")
	[[nodiscard]] int32 GetCurrentValue() const { return CurrentValue; }

	/** Get the starting int32 value. Set after the first tick */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category= "Tween|Info")
	[[nodiscard]] int32 GetStartValue() const { return StartValue.Get(CurrentValue); }
private:
	/** Starting function returning int32. */
	FNativeIntGetter From;

	/** Target function returning int32. */
	FNativeIntGetter To;

	/** Starting value. */
	TOptional<int32> StartValue;

	/** Function to set the interpolated FVector value. */
	FNativeIntSetter SetterFunction;

	/** Current interpolated value. */
	int32 CurrentValue = 0;
};