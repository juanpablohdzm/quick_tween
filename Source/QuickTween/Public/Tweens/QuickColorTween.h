// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickColorTween.generated.h"

class UQuickColorTween;
DECLARE_DELEGATE_RetVal_OneParam(FColor, FNativeColorGetter, UQuickColorTween*);
DECLARE_DELEGATE_TwoParams( FNativeColorSetter, const FColor&, UQuickColorTween* );

/**
 * Tween class for interpolating between two FColor values over time.
 * Inherits from UQuickTweenBase and provides vector-specific tweening functionality.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickColorTween : public UQuickTweenBase
{
	GENERATED_BODY()
private:
	UQuickColorTween() = default;

	/**
	 * Set up the vector2D tween with the specified parameters.
	 */
	void SetUp(
		const UObject* worldContextObject,
		FNativeColorGetter from,
		FNativeColorGetter to,
		FNativeColorSetter setter,
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
	 * Create a new UQuickColorTween instance and initialize it.
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
	static UQuickColorTween* CreateTween(
		UObject* worldContextObject,
		FNativeColorGetter from,
		FNativeColorGetter to,
		FNativeColorSetter setter,
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
			UE_LOG(LogQuickTweenBase, Warning, TEXT("UQuickColorTween::CreateTween: One or more delegate functions are not bound."));
			return nullptr;
		}
		
		UQuickColorTween* tween = NewObject<UQuickColorTween>(worldContextObject);
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

	/** Get the current interpolated FColor value. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FColor GetCurrentValue() const { return CurrentValue; }

	/** Get the starting FColor value. Set after the first tick */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FColor GetStartValue() const { return StartValue.Get(FColor::White); }

	/** Get the ending FColor value. Set after the first tick */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FColor GetEndValue() const { return EndValue.Get(FColor::White); }
protected:
	virtual void ApplyAlphaValue(float alpha) override;

	virtual void HandleOnStart() override;

	virtual void HandleOnComplete() override;

private:
	/** Starting function returning FColor. */
	FNativeColorGetter From;

	/** Target function returning FColor. */
	FNativeColorGetter To;

	/** Starting value. */
	TOptional<FColor> StartValue;

	/** Target value. */
	TOptional<FColor> EndValue;

	/** Function to set the interpolated FColor value. */
	FNativeColorSetter Setter;

	/** Current interpolated value. */
	FColor CurrentValue;
};