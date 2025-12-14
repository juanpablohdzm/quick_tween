// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Utils/EaseType.h"
#include "Utils/LoopType.h"
#include "QuickTweenable.generated.h"

/**
 * Base UObject for QuickTween tweens.
 *
 * Provides a common interface for playback control, querying state, and configuring
 * tween behavior. This class is intended to be extended in C++ or Blueprint to
 * implement concrete tween logic (e.g., property interpolation). Marked as
 * BlueprintType so instances and subclasses can be used in Blueprint graphs.
 *
 * Usage notes:
 *  - Use SetOwner to assign the owning sequence.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickTweenable : public UObject
{
	GENERATED_BODY()
public:

	/** Set the owning tween object for this tweenable. */
	virtual void SetOwner(UQuickTweenable* owner);

	/** Start or resume playback of the tween.
	 * @param instigator Optional pointer indicating which tween initiated the call, if tween is in sequence only the owner can run the function.
	 */
	UFUNCTION(BlueprintCallable, Category= "Tween|Control")
	virtual void Play(UQuickTweenable* instigator = nullptr);

	/** Pause playback of the tween.
	 * @param instigator Optional pointer indicating which tween initiated the call, if tween is in sequence only the owner can run the function.
	 */
	UFUNCTION(BlueprintCallable, Category= "Tween|Control")
	virtual void Pause(UQuickTweenable* instigator = nullptr);

	/** Play the tween in reverse (playback direction is inverted).
	 * @param instigator Optional pointer indicating which tween initiated the call, if tween is in sequence only the owner can run the function.
	 */
	UFUNCTION(BlueprintCallable, Category= "Tween|Control")
	virtual void Reverse(UQuickTweenable* instigator = nullptr);

	/** Restart the tween from its beginning (in case it is reversed, it will restart at the end).
	 *  Must be played again after restarting.
	 * @param instigator Optional pointer indicating which tween initiated the call, if tween is in sequence only the owner can run the function.
	 */
	UFUNCTION(BlueprintCallable, Category= "Tween|Control")
	virtual void Restart(UQuickTweenable* instigator = nullptr);

	/** Immediately complete the tween.
	 *  If the tween is reversed bSnapToEnd is inverted.
	 * @param instigator Optional pointer indicating which tween initiated the call, if tween is in sequence only the owner can run the function.
	 * @param bSnapToEnd If true, snap properties to final state when completing.
	 */
	UFUNCTION(BlueprintCallable, Category= "Tween|Control")
	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true);

	/** Forcefully kill the tween and mark it for removal.
	 * @param instigator Optional pointer indicating which tween initiated the call, if tween is in sequence only the owner can run the function.
	 */
	UFUNCTION(BlueprintCallable, Category= "Tween|Control")
	virtual void Kill(UQuickTweenable* instigator = nullptr);

	/** Update the tween state by a time delta.
	 * @param deltaTime Time in seconds to advance the tween.
	 * @param instigator Optional pointer indicating which tween initiated the call, if tween is in sequence only the owner can run the function.
	 */
	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr);

	/**
	 * Query whether this tween is pending removal from the manager.
	 * @return True if the tween is pending kill, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsPendingKill() const;

	/** Get the configured duration of the tween in seconds. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual float GetDuration() const;

	/** Get the total elapsed time since the tween started (in seconds). */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual float GetElapsedTime() const;

	/** Get the current time scale applied to the tween (1.0 = normal speed). */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual float GetTimeScale() const;

	/** Check if the tween is currently playing.
	 * @return True when playing; false when paused or stopped.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsPlaying() const;

	/** Check whether the tween has reached its completion state. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsCompleted() const;

	/** Returns true if the tween's reversed flag is set (affects playback direction). */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsReversed() const;

	/** Get the easing function type used by this tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual EEaseType GetEaseType() const;

	/** Get the custom easing curve used by the tween, if any.
	 * @return Pointer to a UCurveFloat or nullptr if not set.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual UCurveFloat* GetEaseCurve() const;

	/** Get the configured number of loops for this tween (-1 for infinite if supported). */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual int32 GetLoops() const;

	/** Get the loop behavior used when the tween repeats (e.g., Restart, Yoyo). */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual ELoopType GetLoopType() const;

	/** Get the user-assigned tag for this tween. Useful for identification and grouping. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual FString GetTweenTag() const;

	/** Get the current loop index (0-based) the tween is on.
	 * @return 0 for the first loop, 1 for the second, etc.
	 */
	UFUNCTION(BlueprintCallable, Category= "Tween|Info")
	[[nodiscard]] virtual int32 GetCurrentLoop() const;

	/** Returns whether this tween will be removed automatically after completion. */
	UFUNCTION(BlueprintCallable, Category= "Tween|Info")
	[[nodiscard]] virtual bool GetAutoKill() const;

	/** Whether the tween should continue running even when the game is paused. */
	UFUNCTION(BlueprintCallable, Category= "Tween|Info")
	[[nodiscard]] virtual bool GetShouldPlayWhilePaused() const;
};


