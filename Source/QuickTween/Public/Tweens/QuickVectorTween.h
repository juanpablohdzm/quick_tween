// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickVectorTween.generated.h"

class UQuickVectorTween;
DECLARE_DELEGATE_RetVal_OneParam(FVector, FNativeVectorGetter, UQuickVectorTween*);
DECLARE_DELEGATE_TwoParams( FNativeVectorSetter, const FVector&, UQuickVectorTween* );

/**
 * Tween class for interpolating between two FVector values over time.
 * Inherits from UQuickTweenBase and provides vector-specific tweening functionality.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickVectorTween : public UQuickTweenBase
{
	GENERATED_BODY()
private:
	UQuickVectorTween() = default;

	/**
	 * Set up the vector tween with the specified parameters.
	 */
	void SetUp(
		FNativeVectorGetter from,
		FNativeVectorGetter to,
		FNativeVectorSetter setterFunction,
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
	 * Create a new UQuickVectorTween instance and initialize it.
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
	static UQuickVectorTween* CreateTween(
		FNativeVectorGetter from,
		FNativeVectorGetter to,
		FNativeVectorSetter setterFunction,
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
		UQuickVectorTween* tween = NewObject<UQuickVectorTween>();
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


public:
	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr) override;

	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true) override;

	/** Get the current interpolated FVector value. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FVector GetCurrentValue() const { return CurrentValue; }

	/** Get the starting FVector value. Set after the first tick */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FVector GetStartValue() const { return StartValue.Get(FVector::ZeroVector); }
private:
	/** Starting function returning FVector. */
	FNativeVectorGetter From;

	/** Target function returning FVector. */
	FNativeVectorGetter To;

	/** Starting value. */
	TOptional<FVector> StartValue;

	/** Function to set the interpolated FVector value. */
	FNativeVectorSetter SetterFunction;

	/** Current interpolated value. */
	FVector CurrentValue;
};