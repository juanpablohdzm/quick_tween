// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenable.h"
#include "../Utils/EaseType.h"
#include "../Utils/LoopType.h"
#include "QuickTweenBase.generated.h"

class UQuickTweenSequence;
class UQuickTweenBase;

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
	 * @param duration Duration of the tween in seconds.
	 * @param timeScale Time scale multiplier.
	 * @param easeType Type of easing to use.
	 * @param easeCurve Optional custom curve for easing.
	 * @param loops Number of loops (0 = infinite).
	 * @param loopType Looping behavior.
	 * @param tweenTag Optional tag for identification.
	 * @param worldContextObject Context object for world access.
	 * @param bShouldAutoKill Whether to auto-kill the tween on completion.
	 * @param bShouldPlayWhilePaused Whether the tween should play while the game is paused.
	 * @param bShouldAutoPlay Whether the tween should start playing immediately.
	 */
	void SetUp(
		float duration,
		float timeScale,
		EEaseType easeType,
		UCurveFloat* easeCurve,
		int32 loops,
		ELoopType loopType,
		const FString& tweenTag = FString(),
		const UObject* worldContextObject = nullptr,
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	virtual void SetOwner(UQuickTweenable* owner) override { Owner = owner; }

#pragma endregion

#pragma region Tween Control
public:

	virtual void Play(UQuickTweenable* instigator = nullptr) override;

	virtual void Pause(UQuickTweenable* instigator = nullptr) override;

	virtual void Stop(UQuickTweenable* instigator = nullptr) override;

	virtual void Reverse(UQuickTweenable* instigator = nullptr) override;

	virtual void Restart(UQuickTweenable* instigator = nullptr) override;

	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true) override;

	virtual void Kill(UQuickTweenable* instigator = nullptr) override;

	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr) override;

	virtual void SetAutoKill(bool bShouldAutoKill, UQuickTweenable* instigator = nullptr) override;
private:

	void Update_Restart(float deltaTime, UQuickTweenable* instigator);


	void Update_PingPong(float deltaTime, UQuickTweenable* instigator);

#pragma endregion

#pragma region Tween State Queries
public:
	/**
	 * If this tween should be eliminated from the manager.
	 * @return True if the tween is pending kill, false otherwise.
	 */
	virtual bool GetIsPendingKill() const override { return bIsPendingKill; }

	/** Gets the duration of the tween. */
	[[nodiscard]] virtual float GetDuration() const override { return Duration;}

	[[nodiscard]] virtual float GetElapsedTime() const override { return ElapsedTime; }

	/** Gets the time scale of the tween. */
	[[nodiscard]] virtual float GetTimeScale() const override { return TimeScale; }

	/** Returns true if the tween is currently playing. */
	[[nodiscard]] virtual bool GetIsPlaying() const override { return bIsPlaying; }

	/** Returns true if the tween is completed. */
	[[nodiscard]] virtual bool GetIsCompleted() const override { return bIsCompleted; }

	/** Returns true if the tween is playing backwards. */
	[[nodiscard]] virtual bool GetIsBackwards() const override { return bIsBackwards; }

	[[nodiscard]] virtual bool GetIsReversed() const override { return bIsReversed; }

	/** Gets the ease type of the tween. */
	[[nodiscard]] virtual EEaseType GetEaseType() const override { return EaseType; }

	/** Gets the custom ease curve of the tween, if any. */
	[[nodiscard]] virtual UCurveFloat* GetEaseCurve() const override { return EaseCurve; }

	/** Gets the number of loops for the tween. */
	[[nodiscard]] virtual int32 GetLoops() const override{ return Loops; }

	/** Gets the loop type of the tween. */
	[[nodiscard]] virtual ELoopType GetLoopType() const override { return LoopType; }

	/** Gets the tag assigned to the tween. */
	[[nodiscard]] virtual FString GetTweenTag() const override { return TweenTag; }

	/** Gets the current loop index (1-based). */
	[[nodiscard]] virtual int32 GetCurrentLoop() const override { return CurrentLoop; }

	/** Gets whether this tween will be removed after completion. */
	[[nodiscard]] virtual bool GetAutoKill() const override { return bAutoKill; }

	/** Gets whether this tween should play while paused. */
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


	/** Event triggered when the tween starts. */
	FNativeDelegateTween OnStart;

	/** Event triggered when the tween updates. */
	FNativeDelegateTween OnUpdate;

	/** Event triggered when the tween completes. */
	FNativeDelegateTween OnComplete;

	/** Event triggered when the tween is killed. */
	FNativeDelegateTween OnKilled;

	/** Time elapsed since the tween started. */
	float ElapsedTime = 0.0f;

private:
	/** Duration of the tween in seconds. */
	float Duration = 0.0f;
	/** Time scale multiplier. */
	float TimeScale = 1.0f;

	/** Whether the tween has started. */
	bool bHasStarted = false;

	/** Whether the tween is currently playing. */
	bool bIsPlaying = false;

	/** Whether the tween is completed. */
	bool bIsCompleted = false;

	/** Whether the tween is playing backwards. */
	bool bIsBackwards = false;

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