// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickIntTween.generated.h"

class UQuickIntTween;
DECLARE_DELEGATE_RetVal_OneParam(int32, FNativeIntGetter, UQuickIntTween*);
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
		const UObject* worldContextObject,
		FNativeIntGetter from,
		FNativeIntGetter to,
		FNativeIntSetter setter,
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
	 * Create and set up a UQuickIntTween with the specified parameters.
	 *
	 * Note: The start and end value will be cached at the first update.
	 *
	 * @param worldContextObject Context object for world access.
	 * @param from Function to get the FROM value.
	 * @param to Function to get the TO value.
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
	static UQuickIntTween* CreateTween(
		UObject* worldContextObject,
		FNativeIntGetter from,
		FNativeIntGetter to,
		FNativeIntSetter setter,
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
			UE_LOG(LogQuickTweenBase, Warning, TEXT("UQuickIntTween::CreateTween: One or more delegate functions are not bound."));
			return nullptr;
		}
		
		UQuickIntTween* tween = NewObject<UQuickIntTween>(worldContextObject);
		tween->SetUp(
			worldContextObject,
			MoveTemp(from),
			MoveTemp(to),
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

	/** Get the current interpolated int32 value. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category= "Tween|Info")
	[[nodiscard]] int32 GetCurrentValue() const { return CurrentValue; }

	/** Get the starting int32 value. Set after the first tick */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category= "Tween|Info")
	[[nodiscard]] int32 GetStartValue() const { return StartValue.Get(0); }

 	/** Get the ending int32 value. Set after the first tick */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category= "Tween|Info")
	[[nodiscard]] int32 GetEndValue() const { return EndValue.Get(0); }
protected:
	virtual void ApplyAlphaValue(float alpha) override;

	virtual void HandleOnStart() override;

	virtual void HandleOnComplete() override;

private:
	/** Starting function returning int32. */
	FNativeIntGetter From;

	/** Target function returning int32. */
	FNativeIntGetter To;

	/** Starting value. */
	TOptional<int32> StartValue;

	/** Target value. */
	TOptional<int32> EndValue;

	/** Function to set the interpolated FVector value. */
	FNativeIntSetter Setter;

	/** Current interpolated value. */
	int32 CurrentValue = 0;
};