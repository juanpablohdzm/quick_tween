// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickFloatTween.generated.h"

class UQuickFloatTween;
DECLARE_DELEGATE_RetVal_OneParam(float, FNativeFloatGetter, UQuickFloatTween*);
DECLARE_DELEGATE_TwoParams( FNativeFloatSetter, const float, UQuickFloatTween* );
/**
 * Tween class for interpolating between two FVector values over time.
 * Inherits from UQuickTweenBase and provides vector-specific tweening functionality.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickFloatTween : public UQuickTweenBase
{
	GENERATED_BODY()
private:
	UQuickFloatTween() = default;

	/**
	 * Set up the vector tween with the specified parameters.
	 */
	void SetUp(
		const UObject* worldContextObject,
		FNativeFloatGetter from,
		FNativeFloatGetter to,
		FNativeFloatSetter setter,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = FString(),
		bool bShouldAutoKill = false,
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
	 * Create and set up a UQuickFloatTween with the specified parameters.
	 *
	 * Note: The start value will be cached from the component's current location at the first update.
	 *
	 * @param worldContextObject Context object for world access.
	 * @param from Function to get the FROM value.
	 * @param to Function to get the TO value.
	 * @param setter Function to apply the interpolated value.
	 * @param duration Duration of the tween in seconds.
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
	static UQuickFloatTween* CreateTween(
		UObject* worldContextObject,
		FNativeFloatGetter from,
		FNativeFloatGetter to,
		FNativeFloatSetter setter,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = FString(),
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false)
	{
		if (!from.IsBound() || !to.IsBound() || !setter.IsBound())
		{
			UE_LOG(LogQuickTweenBase, Warning, TEXT("UQuickFloatTween::CreateTween: One or more delegate functions are not bound."));
			return nullptr;
		}
		
		UQuickFloatTween* tween = NewObject<UQuickFloatTween>(worldContextObject);
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

	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr) override;

	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true) override;

	/** Get the current interpolated float value. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category= "Tween|Info")
	[[nodiscard]] float GetCurrentValue() const { return CurrentValue; }

	/** Get the starting float value. Set after the first tick */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category= "Tween|Info")
	[[nodiscard]] float GetStartValue() const { return StartValue.Get(CurrentValue); }
private:
	/** Starting function returning float. */
	FNativeFloatGetter From;

	/** Target function returning float. */
	FNativeFloatGetter To;

	/** Starting value. */
	TOptional<float> StartValue;

	/** Function to set the interpolated FVector value. */
	FNativeFloatSetter Setter;

	/** Current interpolated value. */
	float CurrentValue = 0.0f;
};