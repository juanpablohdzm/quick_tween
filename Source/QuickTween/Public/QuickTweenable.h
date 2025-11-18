// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Utils/EaseType.h"
#include "Utils/LoopType.h"
#include "QuickTweenable.generated.h"

UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickTweenable : public UObject
{
	GENERATED_BODY()
public:

	/** Set the owning tween object for this tweenable. */
	virtual void SetOwner(UQuickTweenable* owner);

	/** Start or resume playback of the tween.
	 * @param instigator Optional pointer indicating which tween initiated the call.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Play(UQuickTweenable* instigator = nullptr);

	/** Pause playback of the tween.
	 * @param instigator Optional pointer indicating which tween initiated the call.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Pause(UQuickTweenable* instigator = nullptr);

	/** Stop playback and reset internal state as appropriate.
	 * @param instigator Optional pointer indicating which tween initiated the call.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Stop(UQuickTweenable* instigator = nullptr);

	/** Play the tween in reverse (playback direction is inverted).
	 * @param instigator Optional pointer indicating which tween initiated the call.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Reverse(UQuickTweenable* instigator = nullptr);

	/** Restart the tween from its beginning.
	 * @param instigator Optional pointer indicating which tween initiated the call.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Restart(UQuickTweenable* instigator = nullptr);

	/** Immediately complete the tween.
	 *  If the tween is reversed bSnapToEnd is inverted.
	 * @param instigator Optional pointer indicating which tween initiated the call.
	 * @param bSnapToEnd If true, snap properties to final state when completing.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true);

	/** Reset the tween to its initial configuration without starting playback.
	 * @param instigator Optional pointer indicating which tween initiated the call.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Reset(UQuickTweenable* instigator = nullptr);

	/** Forcefully kill the tween and mark it for removal.
	 * @param instigator Optional pointer indicating which tween initiated the call.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Kill(UQuickTweenable* instigator = nullptr);

	/** Update the tween state by a time delta.
	 * @param deltaTime Time in seconds to advance the tween.
	 * @param instigator Optional pointer indicating which tween initiated the call.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr);

	/** Configure whether the tween should be automatically removed after completion.
	 * @param bShouldAutoKill Whether to auto-kill when completed.
	 * @param instigator Optional pointer indicating which tween initiated the call.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetAutoKill(bool bShouldAutoKill, UQuickTweenable* instigator = nullptr);

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

	/** Get the normalized progress of the tween between 0.0 and 1.0. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual float GetProgress() const;

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

	/** Returns true if the tween is currently playing backwards in time. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsBackwards() const;

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

	/** Get the current loop index (1-based) the tween is on.
	 * @return 1 for the first loop, 2 for the second, etc.
	 */
	UFUNCTION(BlueprintCallable)
	[[nodiscard]] virtual int32 GetCurrentLoop() const;

	/** Returns whether this tween will be removed automatically after completion. */
	UFUNCTION(BlueprintCallable)
	[[nodiscard]] virtual bool GetAutoKill() const;

	/** Whether the tween should continue running even when the game is paused. */
	UFUNCTION(BlueprintCallable)
	[[nodiscard]] virtual bool GetShouldPlayWhilePaused() const;
};


