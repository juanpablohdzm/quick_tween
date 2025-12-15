// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonValues.h"
#include "QuickTweenable.h"
#include "UObject/Object.h"
#include "../Utils/LoopType.h"
#include "QuickTweenSequence.generated.h"

class UQuickTweenBase;
class UQuickTweenSequence;

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenSequence, Log, All);

DECLARE_MULTICAST_DELEGATE_OneParam(FNativeDelegateTweenSequence, UQuickTweenSequence*);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDynamicDelegateTweenSequence, UQuickTweenSequence*, TweenSequence);


/**
 * Represents a group of tweens that run in parallel.
 */
USTRUCT()
struct FQuickTweenSequenceGroup
{
	GENERATED_BODY()

	/** Tweens that are part of this group. */
	UPROPERTY()
	TArray<UQuickTweenable*> Tweens;

	/** Start time of the group within the sequence. */
	float StartTime = 0.0f;

	/** Max duration of this group. */
	float Duration = 0.0f;
};

/**
 * UQuickTweenSequence manages a sequence of tween animations.
 * Allows joining, appending, and controlling multiple tweens as a group.
 * Supports looping, reversing, and querying sequence state.
 * Note:
 * - Tweens added to the sequence can not be controlled individually.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickTweenSequence : public UQuickTweenable
{
	GENERATED_BODY()

private:
	UQuickTweenSequence() = default;

	/**
	 * Set up the sequence with optional looping parameters.
	 */
	void SetUp(
		const UObject* worldContextObject,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& id = FString(),
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false);

public:
	virtual ~UQuickTweenSequence() override;

#pragma region Sequence Creation

	/**
	 * Creates a new tween sequence with the specified parameters.
	 *
	 * @param worldContextObject Context object for world access.
	 * @param loops Number of loops (0 = infinite).
	 * @param loopType Looping behavior.
	 * @param id Optional identifier for the sequence.
	 * @param bShouldAutoKill Whether to auto-kill the sequence on completion.
	 * @param bShouldPlayWhilePaused Whether the sequence should play while the game is paused.
	 * @return Pointer to the created tween sequence.
	 */
	static UQuickTweenSequence* CreateSequence(
		UObject* worldContextObject,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& id = FString(),
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false)
	{
		UQuickTweenSequence* sequence = NewObject<UQuickTweenSequence>(worldContextObject);
		sequence->SetUp(
			worldContextObject,
			loops,
			loopType,
			id,
			bShouldAutoKill,
			bShouldPlayWhilePaused);
		return sequence;
	}

	/**
	 * Joins a tween to the previously created group, or creates a new one if it is the first.
	 * @param tween The tween to join.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Creation")
	UQuickTweenSequence* Join(UQuickTweenable* tween);

	/**
	 * Creates a new group and adds a new tween to it.
	 * @param tween The tween to append.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Creation")
	UQuickTweenSequence* Append(UQuickTweenable* tween);

	virtual void SetOwner(UQuickTweenable* owner) override { Owner = owner; }
#pragma endregion

#pragma region Sequence Control
public:

	virtual void Play(UQuickTweenable* instigator = nullptr) override;

	virtual void Pause(UQuickTweenable* instigator = nullptr) override;

	virtual void Reverse(UQuickTweenable* instigator = nullptr) override;

	virtual void Complete(UQuickTweenable* instigator = nullptr, bool bSnapToEnd = true) override;

	virtual void Restart(UQuickTweenable* instigator = nullptr) override;

	virtual void Kill(UQuickTweenable* instigator = nullptr) override;

	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr) override;

	[[nodiscard]] virtual bool GetIsPendingKill() const override { return SequenceState == EQuickTweenState::Kill; }
#pragma endregion


#pragma region Sequence State Queries
public:

	[[nodiscard]] virtual bool GetIsPlaying() const override { return SequenceState == EQuickTweenState::Play;}

	[[nodiscard]] virtual float GetTimeScale() const override { return 1.0f; }

	[[nodiscard]] virtual bool GetIsReversed() const override { return bIsReversed; }

	[[nodiscard]] virtual EEaseType GetEaseType() const override { return EEaseType::Linear; }

	[[nodiscard]] virtual UCurveFloat* GetEaseCurve() const override { return nullptr; }

	[[nodiscard]] virtual bool GetAutoKill() const override { return bAutoKill; }

	[[nodiscard]] virtual bool GetIsCompleted() const override { return SequenceState == EQuickTweenState::Complete; }

	[[nodiscard]] virtual float GetLoopDuration() const override;

	[[nodiscard]] virtual float GetTotalDuration() const override
	{
		if (Loops == INFINITE_LOOPS)
		{
			return TNumericLimits<float>::Max();
		}
		return GetLoopDuration() * GetLoops() / GetTimeScale();
	};

	[[nodiscard]] virtual float GetElapsedTime() const override { return ElapsedTime; }

	[[nodiscard]] virtual int32 GetLoops() const override { return Loops; }

	[[nodiscard]] virtual int32 GetCurrentLoop() const override { return CurrentLoop; }

	[[nodiscard]] virtual ELoopType GetLoopType() const override { return LoopType; }

	[[nodiscard]] virtual FString GetTweenTag() const override { return SequenceTweenId; }

	[[nodiscard]] virtual bool GetShouldPlayWhilePaused() const override { return bPlayWhilePaused; }

	/**
	 * Gets the number of tweens in the sequence.
	 * @return Number of tweens.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	[[nodiscard]] int32 GetNumTweens() const;

	/**
	 * Gets the tween at the specified index.
	 * @param index Index of the tween.
	 * @return Pointer to the tween.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	[[nodiscard]] UObject* GetTween(int32 index) const;

#pragma endregion

#pragma region Delegates
public:

	/**
	 * Assign a Blueprint dynamic delegate to be invoked when the tween starts.
	 * @param callback Dynamic delegate with signature (UQuickTweenSequence* Tween).
	 *                 The provided delegate will be stored and called on start events.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void AssignOnStartEvent(FDynamicDelegateTweenSequence callback);

	/**
	 * Assign a Blueprint dynamic delegate to be invoked on every tween update.
	 * @param callback Dynamic delegate with signature (UQuickTweenSequence* Tween).
	 *                 The provided delegate will be stored and called each update tick.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void AssignOnUpdateEvent(FDynamicDelegateTweenSequence callback);

	/**
	 * Assign a Blueprint dynamic delegate to be invoked when the tween completes.
	 * @param callback Dynamic delegate with signature (UQuickTweenSequence* Tween).
	 *                 The provided delegate will be stored and called on completion.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void AssignOnCompleteEvent(FDynamicDelegateTweenSequence callback);

	/**
	 * Assign a Blueprint dynamic delegate to be invoked when the tween is killed.
	 * @param callback Dynamic delegate with signature (UQuickTweenSequence* Tween).
	 *                 The provided delegate will be stored and called when the tween is killed.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void AssignOnKilledEvent(FDynamicDelegateTweenSequence callback);

	/**
	 * Assign a Blueprint dynamic delegate to be invoked when the tween loops.
	 * @param callback Dynamic delegate with signature (UQuickTweenSequence* Tween).
	 *                 The provided delegate will be stored and called when the tween loops.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Event"), Category = "Tween|Info")
	void AssignOnLoopEvent(FDynamicDelegateTweenSequence callback);

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

	/** Called when the sequence starts. */
	FNativeDelegateTweenSequence OnStart;

	/** Called when the sequence updates. */
	FNativeDelegateTweenSequence OnUpdate;

	/** Called when the sequence completes. */
	FNativeDelegateTweenSequence OnComplete;

	/** Called when the sequence is killed. */
	FNativeDelegateTweenSequence OnKilled;

	/** Called when the sequence loops. */
	FNativeDelegateTweenSequence OnLoop;
#pragma endregion

private:
	bool InstigatorIsOwner(UQuickTweenable* instigator) const
	{
		if (!Owner) return true; // No owner means it's not in a sequence
		return instigator == Owner;
	};

	/**
	 * Handle operations required when the sequence transitions to the Idle state.
	 * Typical responsibilities: reset runtime counters, ensure child tweens are stopped,
	 * and prepare the sequence for a fresh start.
	 */
	void HandleOnIdleTransition();

	/**
	 * Handle operations required when the sequence transitions to the Start state.
	 * Typical responsibilities: initialize timing, mark sequence as running, and
	 * invoke start delegates/events.
	 */
	void HandleOnStartTransition();

	/**
	 * Handle operations required when the sequence transitions to the Play state.
	 * Typical responsibilities: resume tween playback, update playing flags, and
	 * ensure child tweens continue from their current time.
	 */
	void HandleOnPlayTransition();

	/**
	 * Handle operations required when the sequence transitions to the Pause state.
	 * Typical responsibilities: pause child tweens, preserve elapsed time, and set
	 * appropriate flags so playback can be resumed later.
	 */
	void HandleOnPauseTransition();

	/**
	 * Handle operations required when the sequence transitions to the Complete state.
	 * Typical responsibilities: finalize tween values (optionally snap to end),
	 * trigger completion delegates/events, and handle looping or auto-kill behavior.
	 */
	void HandleOnCompleteTransition(bool bSnapToEnd = true);

	/**
	 * Handle operations required when the sequence transitions to the Kill state.
	 * Typical responsibilities: stop and cleanup child tweens, release resources,
	 * and invoke killed delegates/events.
	 */
	void HandleOnKillTransition();

	/**
	 * Request a state transition for the sequence.
	 * This should perform validation and scheduling of the transition from the current
	 * SequenceState to the specified newState.
	 *
	 * @param newState The desired state to transition into.
	 */
	template <typename ...Args>
	void RequestStateTransition(EQuickTweenState newState, Args&&... args);

	/** Current state of the sequence. */
	EQuickTweenState SequenceState = EQuickTweenState::Idle;

	/** Array of tween groups in the sequence. */
	UPROPERTY(Transient)
	TArray<FQuickTweenSequenceGroup> TweenGroups;

	/** Elapsed time since the sequence started. */
	float ElapsedTime = 0.0f;

	/** Whether the sequence is playing backwards. */
	bool bIsReversed = false;

	/** Current loop index. */
	int32 CurrentLoop = 1;

	/** Total number of loops. */
	int32 Loops = -1;

	/** Loop type for the sequence. */
	ELoopType LoopType = ELoopType::Restart;

	/** Identifier for the sequence. */
	FString SequenceTweenId;

	/** Index of the current tween group. */
	int32 CurrentTweenGroupIndex = 0;

	/** If the sequence should be eliminated from the manager when completed. */
	bool bAutoKill = true;

	/** If the sequence should play while the game is paused. */
	bool bPlayWhilePaused = false;

	/** Object to do world queries. */
	UPROPERTY(Transient)
	const UObject* WorldContextObject = nullptr;

	UPROPERTY(Transient)
	UQuickTweenable* Owner = nullptr;

};


template <typename ... Args>
void UQuickTweenSequence::RequestStateTransition(EQuickTweenState newState, Args&&... args)
{
	if (newState == SequenceState) return;

	if (ValidTransitions[SequenceState].Contains(newState))
	{
		SequenceState = newState;
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
		UE_LOG(LogQuickTweenSequence, Warning, TEXT("Invalid state transition from %s to %s"),  *UEnum::GetValueAsString(SequenceState), *UEnum::GetValueAsString(newState));
	}
}
