// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <variant>

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickRotatorTween.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
/**
 * Tween class for interpolating between two FRotator values over time.
 * Inherits from UQuickTweenBase and provides rotator-specific tweening functionality.
 */
class QUICKTWEEN_API UQuickRotatorTween : public UQuickTweenBase
{
	GENERATED_BODY()
public:

	/**
	 * Set up the rotator tween with the specified parameters.
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
	 */
	void SetUp(
		TFunction<FRotator()>&& from,
		TFunction<FRotator()>&& to,
		bool bUseShortestPath,
		TFunction<void(const FRotator&)>&& setterFunction,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = FString(),
		const UObject* worldContextObject = nullptr,
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false)
	{
		From = from;
		To = to;
		bShortestPath = bUseShortestPath;
		SetterFunction = setterFunction;
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
			bShouldPlayWhilePaused);

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
	/** Starting value or function returning FRotator. */
	TFunction<FRotator()> From;

	/** Target FRotator function. */
	TFunction<FRotator()> To;

	/** Starting value. */
	TOptional<FRotator> StartValue;

	/** Function to set the interpolated FRotator value. */
	TFunction<void(const FRotator&)> SetterFunction;

	bool bShortestPath = true;
};