// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickRotatorTween.generated.h"

class UQuickRotatorTween;

DECLARE_DELEGATE_RetVal_OneParam(FRotator, FNativeRotatorGetter, UQuickRotatorTween*);
DECLARE_DELEGATE_TwoParams( FNativeRotatorSetter, const FRotator&, UQuickRotatorTween* );

/**
 * Tween class for interpolating between two FRotator values over time.
 * Inherits from UQuickTweenBase and provides rotator-specific tweening functionality.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickRotatorTween : public UQuickTweenBase
{
	GENERATED_BODY()
private:
	UQuickRotatorTween() = default;

	/**
	 * Set up the rotator tween with the specified parameters.
	 */
	void SetUp(
		const UObject* worldContextObject,
		FNativeRotatorGetter from,
		FNativeRotatorGetter to,
		bool bUseShortestPath,
		FNativeRotatorSetter setter,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = FString(),
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false)
	{
		From = MoveTemp(from);
		To = MoveTemp(to);
		Setter = MoveTemp(setter);
		bShortestPath = bUseShortestPath;
		UQuickTweenBase::SetUp(
			worldContextObject,
			duration,
			timeScale,
			easeType,
			easeCurve,
			loops,
			loopType,
			tweenTag,
			bShouldAutoKill,
			bShouldPlayWhilePaused,
			bShouldAutoPlay);

	}
public:

	/**
	 * Creates a new rotator tween with the specified parameters.
	 *
	 * Note: The start and end value will be cached at the first update.
	 *
	 * @param worldContextObject Context object for world access.
	 * @param from Function to get the FROM value.
	 * @param to Function to get the TO value.
	 * @param bUseShortestPath Whether to use the shortest path for interpolation.
	 * @param setter Function to apply the interpolated value.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Multiplier for the tween's speed.
	 * @param easeType Type of easing to apply.
	 * @param easeCurve Optional custom curve for easing.
	 * @param loops Number of times to loop the tween.
	 * @param loopType Type of looping behavior.
	 * @param tweenTag Optional tag for identifying the tween.
	 * @param bShouldAutoKill Whether to auto-kill the tween on completion.
	 * @param bShouldPlayWhilePaused Whether the tween should play while the game is paused.
	 * @param bShouldAutoPlay Whether to start playing the tween immediately after setup.
	 */
	static UQuickRotatorTween* CreateTween(
		UObject* worldContextObject,
		FNativeRotatorGetter from,
		FNativeRotatorGetter to,
		bool bUseShortestPath,
		FNativeRotatorSetter setter,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = FString(),
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false)
	{
		if (!from.IsBound() || !to.IsBound() || !setter.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Warning, TEXT("UQuickRotatorTween::CreateTween: One or more delegate functions are not bound."));
			return nullptr;
		}
		
		UQuickRotatorTween* tween = NewObject<UQuickRotatorTween>(worldContextObject);
		tween->SetUp(
			worldContextObject,
			MoveTemp(from),
			MoveTemp(to),
			bUseShortestPath,
			MoveTemp(setter),
			duration,
			timeScale,
			easeType,
			easeCurve,
			loops,
			loopType,
			tweenTag,
			bShouldAutoKill,
			bShouldPlayWhilePaused,
			bShouldAutoPlay);
		return tween;
	}

	/** Get the current interpolated FRotator value. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FRotator GetCurrentValue() const { return CurrentValue; }

	/** Get the starting FRotator value. Set after the first tick */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FRotator GetStartValue() const { return StartValue.Get(FRotator::ZeroRotator); }

	/** Get the ending FRotator value. Set after the first tick */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FRotator GetEndValue() const { return EndValue.Get(FRotator::ZeroRotator); }
protected:
	virtual void ApplyAlphaValue(float alpha) override;

	virtual void HandleOnStart() override;

	virtual void HandleOnComplete() override;

private:
	/** Starting value or function returning FRotator. */
	FNativeRotatorGetter From;

	/** Target FRotator function. */
	FNativeRotatorGetter To;

	/** Starting value. */
	TOptional<FRotator> StartValue;

	/** Ending value. */
	TOptional<FRotator> EndValue;

	/** Function to set the interpolated FRotator value. */
	FNativeRotatorSetter Setter;

	/** Whether to use the shortest path for interpolation. */
	bool bShortestPath = true;

	/** Current interpolated value. */
	FRotator CurrentValue;
};