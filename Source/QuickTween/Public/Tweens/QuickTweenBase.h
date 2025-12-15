// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonValues.h"
#include "QuickTweenable.h"
#include "../Utils/EaseType.h"
#include "../Utils/LoopType.h"
#include "QuickTweenBase.generated.h"

class UQuickTweenSequence;
class UQuickTweenBase;

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenBase, Log, All);

DECLARE_MULTICAST_DELEGATE_OneParam(FNativeDelegateTween, UQuickTweenBase*);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDynamicDelegateTween, UQuickTweenBase*, Tween);

/**
 * Base class for all QuickTween tweens.
 * Provides core tweening functionality, state, and events.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class QUICKTWEEN_API UQuickTweenBase : public UQuickTweenable
{
	GENERATED_BODY()
public:
	virtual ~UQuickTweenBase();
#pragma region Tween Creation
	/**
	 * Set up the tween with the specified parameters.
	 * @param worldContextObject Context object for world access.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Time scale multiplier.
	 * @param easeType Type of easing to use.
	 * @param easeCurve Optional custom curve for easing.
	 * @param loops Number of loops (0 = infinite).
	 * @param loopType Looping behavior.
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill Whether to auto-kill the tween on completion.
	 * @param bShouldPlayWhilePaused Whether the tween should play while the game is paused.
	 * @param bShouldAutoPlay Whether the tween should start playing immediately.
	 */
	void SetUp(
		const UObject* worldContextObject,
		float duration,
		float timeScale,
		EEaseType easeType,
		UCurveFloat* easeCurve,
		int32 loops,
		ELoopType loopType,
		const FString& tweenTag = FString(),
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	virtual void SetOwner(UQuickTweenable* owner) override { Owner = owner; }

#pragma endregion

#pragma region Tween Control
public:

	virtual void Play(UQuickTweenable* instigator = nullptr) override;

	virtual void Pause(UQuickTweenable* instigator = nullptr) override;

	virtual void Reverse(UQuickTweenable* instigator = nullptr) override;

	virtual void Restart(UQuickTweenable* instigator = nullptr) override;

	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true) override;

	virtual void Kill(UQuickTweenable* instigator = nullptr) override;

	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr) override;

	virtual void Evaluate(float value, bool bIsReversed, UQuickTweenable* instigator = nullptr) override;

#pragma endregion

#pragma region Tween State Queries
public:

	[[nodiscard]] virtual bool GetIsPendingKill() const override { return TweenState == EQuickTweenState::Kill; }

	[[nodiscard]] virtual float GetLoopDuration() const override { return Duration;}

	[[nodiscard]] virtual float GetTotalDuration() const override
	{
		if (Loops == INFINITE_LOOPS)
		{
			return TNumericLimits<float>::Max();
		}
		return GetLoopDuration() * GetLoops() / GetTimeScale();
	}

	[[nodiscard]] virtual float GetElapsedTime() const override { return ElapsedTime; }

	[[nodiscard]] virtual float GetTimeScale() const override { return TimeScale; }

	[[nodiscard]] virtual bool GetIsPlaying() const override { return TweenState == EQuickTweenState::Play; }

	[[nodiscard]] virtual bool GetIsCompleted() const override { return TweenState == EQuickTweenState::Complete; }

	[[nodiscard]] virtual bool GetIsReversed() const override { return bIsReversed; }

	[[nodiscard]] virtual EEaseType GetEaseType() const override { return EaseType; }

	[[nodiscard]] virtual UCurveFloat* GetEaseCurve() const override { return EaseCurve; }

	[[nodiscard]] virtual int32 GetLoops() const override{ return Loops; }

	[[nodiscard]] virtual ELoopType GetLoopType() const override { return LoopType; }

	[[nodiscard]] virtual FString GetTweenTag() const override { return TweenTag; }

	[[nodiscard]] virtual int32 GetCurrentLoop() const override { return CurrentLoop; }

	[[nodiscard]] virtual bool GetAutoKill() const override { return bAutoKill; }

	[[nodiscard]] virtual bool GetShouldPlayWhilePaused() const override {return bPlayWhilePaused;}
#pragma endregion

	bool InstigatorIsOwner(UQuickTweenable* instigator) const
	{
		if (!Owner) return true; // No owner means it's not in a sequence
		return instigator == Owner;
	};

	/**
	 * Assign a Blueprint dynamic delegate to be invoked when the tween starts.
	 * @param callback Dynamic delegate with signature (UQuickTweenBase* Tween).
	 *                 The provided delegate will be stored and called on start events.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void AssignOnStartEvent(FDynamicDelegateTween callback);

	/**
	 * Assign a Blueprint dynamic delegate to be invoked on every tween update.
	 * @param callback Dynamic delegate with signature (UQuickTweenBase* Tween).
	 *                 The provided delegate will be stored and called each update tick.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void AssignOnUpdateEvent(FDynamicDelegateTween callback);

	/**
	 * Assign a Blueprint dynamic delegate to be invoked when the tween completes.
	 * @param callback Dynamic delegate with signature (UQuickTweenBase* Tween).
	 *                 The provided delegate will be stored and called on completion.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void AssignOnCompleteEvent(FDynamicDelegateTween callback);

	/**
	 * Assign a Blueprint dynamic delegate to be invoked when the tween is killed.
	 * @param callback Dynamic delegate with signature (UQuickTweenBase* Tween).
	 *                 The provided delegate will be stored and called when the tween is killed.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void AssignOnKilledEvent(FDynamicDelegateTween callback);

	/**
	 * Assign a Blueprint dynamic delegate to be invoked when the tween loops.
	 * @param callback Dynamic delegate with signature (UQuickTweenBase* Tween).
	 *                 The provided delegate will be stored and called when the tween loops.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void AssignOnLoopEvent(FDynamicDelegateTween callback);

	/**
	 * Remove all bound Blueprint dynamic delegates for the start event that belong to the specified object.
	 * @param object The UObject whose bindings should be removed. If nullptr, no action is taken.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void RemoveAllOnStartEvent(const UObject* object);

	/**
	 * Remove all bound Blueprint dynamic delegates for the update event that belong to the specified object.
	 * @param object The UObject whose bindings should be removed. If nullptr, no action is taken.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void RemoveAllOnUpdateEvent(const UObject* object);

	/**
	 * Remove all bound Blueprint dynamic delegates for the complete event that belong to the specified object.
	 * @param object The UObject whose bindings should be removed. If nullptr, no action is taken.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void RemoveAllOnCompleteEvent(const UObject* object);

	/**
	 * Remove all bound Blueprint dynamic delegates for the killed event that belong to the specified object.
	 * @param object The UObject whose bindings should be removed. If nullptr, no action is taken.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void RemoveAllOnKilledEvent(const UObject* object);

	/**
	 * Remove all bound Blueprint dynamic delegates for the loop event that belong to the specified object.
	 * @param object The UObject whose bindings should be removed. If nullptr, no action is taken.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void RemoveAllOnLoopEvent(const UObject* object);

	/** Event triggered when the tween starts. */
	FNativeDelegateTween OnStart;

	/** Event triggered when the tween updates. */
	FNativeDelegateTween OnUpdate;

	/** Event triggered when the tween completes. */
	FNativeDelegateTween OnComplete;

	/** Event triggered when the tween is killed. */
	FNativeDelegateTween OnKilled;

	/** Event triggered when the tween loops. */
	FNativeDelegateTween OnLoop;

protected:

	/**
	 * Apply the interpolated alpha value to the tweened property.
	 * @param alpha Interpolated alpha value (0.0 to 1.0).
	 */
	virtual void ApplyAlphaValue(float alpha);

	/**
	 * Called when the tween transitions to the Idle state.
	 *
	 * Implementations should reset transient state and ensure the tween is ready
	 * to be started again (e.g. reset elapsed time, clear transient flags).
	 */
	virtual void HandleOnIdleTransition();

	/**
	 * Called when the tween transitions to the Start state.
	 *
	 * This is invoked once when the tween begins. Implementations should perform
	 * any initialization required before the first update tick (e.g. set initial
	 * values, invoke start events).
	 */
	virtual void HandleOnStartTransition();

	/**
	 * Called when the tween transitions to the Play state.
	 *
	 * Invoked when the tween moves into the playing state (can be after Start or
	 * after Resume). Implementations should ensure the tween continues updating.
	 */
	virtual void HandleOnPlayTransition();

	/**
	 * Called when the tween transitions to the Pause state.
	 *
	 * Invoked when playback is paused. Implementations should suspend updates and
	 * preserve state necessary to resume playback later.
	 */
	virtual void HandleOnPauseTransition();

	/**
	 * Called when the tween transitions to the Complete state.
	 *
	 * @param bSnapToEnd If true, the tween should immediately apply the final
	 *                   value/state (snap to end) before running completion logic.
	 *                   If false, complete without forcing the final value.
	 */
	virtual void HandleOnCompleteTransition(bool bSnapToEnd = true);

	/**
	 * Called when the tween transitions to the Kill state.
	 *
	 * Implementations should perform any final cleanup, release resources, and
	 * ensure no further updates or events are dispatched for this tween.
	 */
	virtual void HandleOnKillTransition();
private:

	template <typename ...Args>
	void RequestStateTransition(EQuickTweenState newState, Args&&... args);

	/** Current state of the tween. */
	EQuickTweenState TweenState = EQuickTweenState::Idle;

	/** Time elapsed since the tween started. */
	float ElapsedTime = 0.0f;

	/** Duration of the tween in seconds. */
	float Duration = 0.0f;

	/** Time scale multiplier. */
	float TimeScale = 1.0f;

	/** Internal flag to track if the tween is reversed. */
	bool bIsReversed = false;

	/** Easing type for the tween. */
	EEaseType EaseType = EEaseType::Linear;

	/** Custom curve for easing, if any. */
	UPROPERTY(Transient)
	UCurveFloat* EaseCurve = nullptr;

	/** Current loop index (0-based). */
	int32 CurrentLoop = 0;

	/** Number of loops (-1 = infinite). */
	int32 Loops = -1;

	/** Looping behavior. */
	ELoopType LoopType = ELoopType::Restart;

	/** Optional tag for identifying the tween. */
	FString TweenTag = FString();

	/** If this tween has an owner */
	UPROPERTY()
	UQuickTweenable* Owner = nullptr;

	/** If the tween should auto-kill upon completion. */
	bool bAutoKill = true;

	/** If the tween should play while the game is paused. */
	bool bPlayWhilePaused = false;

	UPROPERTY()
	const UObject* WorldContextObject = nullptr;
};

template <typename ... Args>
void UQuickTweenBase::RequestStateTransition(EQuickTweenState newState, Args&&... args)
{
	if (newState == TweenState) return;

	if (ValidTransitions[TweenState].Contains(newState))
	{
		TweenState = newState;
		switch (newState)
		{
		case EQuickTweenState::Idle:
			HandleOnIdleTransition();
			break;
		case EQuickTweenState::Start:
			HandleOnStartTransition();
			break;
		case EQuickTweenState::Play:
			HandleOnPlayTransition();
			break;
		case EQuickTweenState::Pause:
			HandleOnPauseTransition();
			break;
		case EQuickTweenState::Complete:
			HandleOnCompleteTransition(Forward<Args>(args)...);
			break;
		case EQuickTweenState::Kill:
			HandleOnKillTransition();
			break;
		}
	}
	else
	{
		UE_LOG(LogQuickTweenBase, Warning, TEXT("Invalid state transition from %s to %s"),  *UEnum::GetValueAsString(TweenState), *UEnum::GetValueAsString(newState));
	}
}
