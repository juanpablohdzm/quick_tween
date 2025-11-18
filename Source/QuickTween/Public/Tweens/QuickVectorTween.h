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
	 * Set up the vector tween with the specified parameters.
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
	 */
	void SetUp(
		TFunction<FVector()>&& from,
		TFunction<FVector()>&& to,
		TFunction<void(const FVector&)>&& setterFunction,
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


	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr) override;

	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true) override;
private:
	/** Starting function returning FVector. */
	TFunction<FVector()> From;

	/** Target function returning FVector. */
	TFunction<FVector()> To;

	/** Starting value. */
	TOptional<FVector> StartValue;

	/** Function to set the interpolated FVector value. */
	TFunction<void(const FVector&)> SetterFunction;
};