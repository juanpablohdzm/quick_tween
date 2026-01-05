// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickEmptyTween.generated.h"



/**
 * A lightweight "empty" tween used as a timing placeholder.
 *
 * This tween does not interpolate any property; it only provides timing.
 * Useful for sequencing or delaying actions without animating values.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickEmptyTween : public UQuickTweenBase
{
	GENERATED_BODY()
private:
	UQuickEmptyTween() = default;

	/**
	 * Configure the tween instance.
	 */
	void SetUp(
		const UObject* worldContextObject,
		float duration = 1.0f,
		const FString& tweenTag = FString(),
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false)
	{
		UQuickTweenBase::SetUp(
			worldContextObject,
			duration,
			1.0f,
			EEaseType::Linear,
			nullptr,
			1,
			ELoopType::Restart,
			tweenTag,
			bShouldAutoKill,
			bShouldPlayWhilePaused,
			bShouldAutoPlay);

	}
public:

	/**
	 * Create a new UQuickEmptyTween instance and initialize it.
	 *
	 * @param worldContextObject Context object for world access.
	 * @param duration Duration of the loop in seconds.
	 * @param tweenTag Optional tag for identifying the tween.
	 * @param bShouldAutoKill Whether to auto-kill the tween on completion.
	 * @param bShouldPlayWhilePaused Whether the tween should play while the game is paused.
	 * @param bShouldAutoPlay Whether to start playing the tween immediately after setup.
	 */
	static UQuickEmptyTween* CreateTween(
		UObject* worldContextObject,
		float duration = 1.0f,
		const FString& tweenTag = FString(),
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false)
	{
		UQuickEmptyTween* tween = NewObject<UQuickEmptyTween>(worldContextObject);
		tween->SetUp(
			worldContextObject,
			duration,
			tweenTag,
			bShouldAutoKill,
			bShouldPlayWhilePaused,
			bShouldAutoPlay);
		return tween;
	}
protected:
	virtual void ApplyAlphaValue(float alpha) override;
};