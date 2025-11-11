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

	virtual void SetOwner(UQuickTweenable* owner);

	UFUNCTION(BlueprintCallable)
	virtual void Play(UQuickTweenable* instigator = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void Pause(UQuickTweenable* instigator = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void Stop(UQuickTweenable* instigator = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void Reverse(UQuickTweenable* instigator = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void Restart(UQuickTweenable* instigator = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true);

	UFUNCTION(BlueprintCallable)
	virtual void Reset(UQuickTweenable* instigator = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void Kill(UQuickTweenable* instigator = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void SetAutoKill(bool bShouldAutoKill, UQuickTweenable* instigator = nullptr);

	/**
	 * If this tween should be eliminated from the manager.
	 * @return True if the tween is pending kill, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsPendingKill() const;

	/** Gets the duration of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual float GetDuration() const;

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual float GetElapsedTime() const;

	/** Gets the current progress of the tween (0-1). */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual float GetProgress() const;

	/** Gets the time scale of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual float GetTimeScale() const;

	/** Returns true if the tween is currently playing. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsPlaying() const;

	/** Returns true if the tween is completed. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsCompleted() const;

	/** Returns true if the tween is playing backwards. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsBackwards() const;

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsReversed() const;

	/** Gets the ease type of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual EEaseType GetEaseType() const;

	/** Gets the custom ease curve of the tween, if any. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual UCurveFloat* GetEaseCurve() const;

	/** Gets the number of loops for the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual int32 GetLoops() const;

	/** Gets the loop type of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual ELoopType GetLoopType() const;

	/** Gets the tag assigned to the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual FString GetTweenTag() const;

	/** Gets the current loop index (1-based). */
	UFUNCTION(BlueprintCallable)
	[[nodiscard]] virtual int32 GetCurrentLoop() const;

	/** Gets whether this tween will be removed after completion. */
	UFUNCTION(BlueprintCallable)
	[[nodiscard]] virtual bool GetAutoKill() const;

	/** Gets whether this tween should play while paused. */
	UFUNCTION(BlueprintCallable)
	[[nodiscard]] virtual bool GetShouldPlayWhilePaused() const;
};


