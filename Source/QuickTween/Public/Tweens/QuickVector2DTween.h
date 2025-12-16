// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickVector2DTween.generated.h"

class UQuickVector2DTween;
DECLARE_DELEGATE_RetVal_OneParam(FVector2D, FNativeVector2DGetter, UQuickVector2DTween*);
DECLARE_DELEGATE_TwoParams( FNativeVector2DSetter, const FVector2D&, UQuickVector2DTween* );

/**
 * Tween class for interpolating between two FVector2D values over time.
 * Inherits from UQuickTweenBase and provides vector-specific tweening functionality.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickVector2DTween : public UQuickTweenBase
{
	GENERATED_BODY()
private:
	UQuickVector2DTween() = default;

	/**
	 * Set up the vector2D tween with the specified parameters.
	 */
	void SetUp(
		const UObject* worldContextObject,
		FNativeVector2DGetter from,
		FNativeVector2DGetter to,
		FNativeVector2DSetter setter,
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
	 * Create a new UQuickVector2DTween instance and initialize it.
	 *
	 * Note: The start value will be cached from the component's current location at the first update.
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
	static UQuickVector2DTween* CreateTween(
		UObject* worldContextObject,
		FNativeVector2DGetter from,
		FNativeVector2DGetter to,
		FNativeVector2DSetter setter,
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
			UE_LOG(LogQuickTweenBase, Warning, TEXT("UQuickVector2DTween::CreateTween: One or more delegate functions are not bound."));
			return nullptr;
		}
		
		UQuickVector2DTween* tween = NewObject<UQuickVector2DTween>(worldContextObject);
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

	/** Get the current interpolated FVector2D value. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FVector2D GetCurrentValue() const { return CurrentValue; }

	/** Get the starting FVector2D value. Set after the first tick */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FVector2D GetStartValue() const { return StartValue.Get(FVector2D::ZeroVector); }

protected:
	virtual void ApplyAlphaValue(float alpha) override;

	virtual void HandleOnStart() override;

	virtual void HandleOnComplete() override;

private:
	/** Starting function returning FVector. */
	FNativeVector2DGetter From;

	/** Target function returning FVector. */
	FNativeVector2DGetter To;

	/** Starting value. */
	TOptional<FVector2D> StartValue;

	/** Function to set the interpolated FVector value. */
	FNativeVector2DSetter Setter;

	/** Current interpolated value. */
	FVector2D CurrentValue;
};