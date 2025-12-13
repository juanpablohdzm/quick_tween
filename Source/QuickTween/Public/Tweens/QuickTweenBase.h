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
	 * @param duration Duration of the tween in seconds.
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

#pragma endregion

#pragma region Tween State Queries
public:

	virtual bool GetIsPendingKill() const override { return bIsPendingKill; }

	[[nodiscard]] virtual float GetDuration() const override { return Duration;}

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

	/** Apply the current alpha value to the tweened property. */
	virtual void ApplyAlphaValue(float alpha);
private:

	void RequestStateTransition(EQuickTweenState newState;

	/** Current state of the tween. */
	EQuickTweenState TweenState;

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

	/** Current loop index (1-based). */
	int32 CurrentLoop = 1;

	/** Number of loops (-1 = infinite). */
	int32 Loops = -1;

	/** Looping behavior. */
	ELoopType LoopType = ELoopType::Restart;

	/** Optional tag for identifying the tween. */
	FString TweenTag;

	/** If this tween has an owner */
	UPROPERTY()
	UQuickTweenable* Owner = nullptr;

	/** If this tween should be eliminated from the manager. */
	bool bIsPendingKill = false;

	/** If the tween should auto-kill upon completion. */
	bool bAutoKill = true;

	/** If the tween should play while the game is paused. */
	bool bPlayWhilePaused = false;

	UPROPERTY()
	const UObject* WorldContextObject = nullptr;
};