// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickVectorTween.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
/**
 * Tween class for interpolating between two FVector values over time.
 * Inherits from UQuickTweenBase and provides vector-specific tweening functionality.
 */
class QUICKTWEEN_API UQuickVectorTween : public UQuickTweenBase
{
	GENERATED_BODY()
public:

	/**
	 * Initializes the vector tween with the specified parameters.
	 *
	 * @param from The starting FVector value.
	 * @param to The target FVector value.
	 * @param setterFunction Function to apply the interpolated value.
	 * @param duration Duration of the tween in seconds.
	 * @param timeScale Multiplier for the tween's speed.
	 * @param easeType Type of easing to apply.
	 * @param easeCurve Optional custom curve for easing.
	 * @param loops Number of times to loop the tween.
	 * @param loopType Type of looping behavior.
	 * @param tweenTag Optional tag for identifying the tween.
	 */
	void Initialize(
		const FVector& from,
		const FVector& to,
		TFunction<void(const FVector&)> setterFunction,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = FString())
	{
		From = from;
		To = to;
		SetterFunction = setterFunction;
		UQuickTweenBase::Initialize(
			duration,
			timeScale,
			easeType,
			easeCurve,
			loops,
			loopType,
			tweenTag);

	}

	/**
	 * Updates the tween's progress based on the elapsed time.
	 *
	 * @param deltaTime Time elapsed since the last update.
	 * @param badge Badge pointer to allow internal calls from sequences.
	 */
	virtual void Update(float deltaTime, Badge<UQuickTweenSequence>* badge) override;

	/** Expose base class Update for external calls without badge. */
	using UQuickTweenBase::Update;

	/**
	 * Completes the tween from a sequence context.
	 *
	 * @param badge Badge pointer to allow internal calls from sequences.
	 * @return Pointer to the completed tween base.
	 */
	virtual UQuickTweenBase* Complete(Badge<UQuickTweenSequence>* badge) override;

	/** Expose base class Complete for external calls without badge. */
	using UQuickTweenBase::Complete;

private:
	/** Starting FVector value. */
	FVector From;

	/** Target FVector value. */
	FVector To;

	/** Function to set the interpolated FVector value. */
	TFunction<void(const FVector&)> SetterFunction;
};