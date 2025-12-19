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
		const UObject* worldContextObject,
		FNativeVectorGetter from,
		FNativeVectorGetter to,
		FNativeVectorSetter setter,
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
	 * Create a new UQuickVectorTween instance and initialize it.
	 *
	 * Note: The start value and end value will be cached at the first update.
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
	static UQuickVectorTween* CreateTween(
		UObject* worldContextObject,
		FNativeVectorGetter from,
		FNativeVectorGetter to,
		FNativeVectorSetter setter,
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
			UE_LOG(LogQuickTweenBase, Warning, TEXT("UQuickVectorTween::CreateTween: One or more delegate functions are not bound."));
			return nullptr;
		}

		UQuickVectorTween* tween = NewObject<UQuickVectorTween>(worldContextObject);
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

	/** Get the current interpolated FVector value. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FVector GetCurrentValue() const { return CurrentValue; }

	/** Get the starting FVector value. Set after the first tick */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FVector GetStartValue() const { return StartValue.Get(FVector::ZeroVector); }

	/** Get the ending FVector value. Set after the first tick */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FVector GetEndValue() const { return EndValue.Get(FVector::ZeroVector); }

protected:
	virtual void ApplyAlphaValue(float alpha) override;

	virtual void HandleOnStart() override;

	virtual void HandleOnComplete() override;

private:
	/** Starting function returning FVector. */
	FNativeVectorGetter From;

	/** Target function returning FVector. */
	FNativeVectorGetter To;

	/** Starting value. */
	TOptional<FVector> StartValue;

	/** Ending value. */
	TOptional<FVector> EndValue;

	/** Function to set the interpolated FVector value. */
	FNativeVectorSetter Setter;

	/** Current interpolated value. */
	FVector CurrentValue;
};