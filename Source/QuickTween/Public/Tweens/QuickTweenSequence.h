// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenable.h"
#include "UObject/Object.h"
#include "../Utils/LoopType.h"
#include "QuickTweenSequence.generated.h"

class UQuickTweenBase;
class UQuickTweenSequence;

DECLARE_MULTICAST_DELEGATE_OneParam(FNativeDelegateTweenSequence, UQuickTweenSequence*);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDynamicDelegateTweenSequence, UQuickTweenSequence*, TweenSequence);


/**
 * Represents a group of tweens that run in parallel.
 */
USTRUCT()
struct FQuickTweenSequenceGroup
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<UQuickTweenable*> Tweens;
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

private:
	/**
	 * Reverses all tweens in the sequence.
	 * @return Reference to this sequence.
	 */
	void Reverse_Tweens();

	/**
	 * Updates the sequence in Restart loop mode.
	 * @param deltaTime Time since last update.
	 * @param instigator
	 */
	void Update_Restart(float deltaTime, UQuickTweenable* instigator);

	/**
	 * Updates the sequence in PingPong loop mode.
	 * @param deltaTime Time since last update.
	 * @param instigator
	 */
	void Update_PingPong(float deltaTime, UQuickTweenable* instigator);
public:

	[[nodiscard]] virtual bool GetIsPendingKill() const override { return bIsPendingKill; }
#pragma endregion


#pragma region Sequence State Queries
public:

	[[nodiscard]] virtual bool GetIsPlaying() const override { return bIsPlaying;}

	[[nodiscard]] virtual float GetTimeScale() const override { return 1.0f; }

	[[nodiscard]] virtual bool GetIsBackwards() const override { return bIsBackwards; }

	[[nodiscard]] virtual bool GetIsReversed() const override { return bIsReversed; }

	[[nodiscard]] virtual EEaseType GetEaseType() const override { return EEaseType::Linear; }

	[[nodiscard]] virtual UCurveFloat* GetEaseCurve() const override { return nullptr; }

	[[nodiscard]] virtual bool GetAutoKill() const override { return bAutoKill; }

	[[nodiscard]] virtual bool GetIsCompleted() const override { return bIsCompleted; }

	[[nodiscard]] virtual float GetDuration() const override;

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
protected:
	bool InstigatorIsOwner(UQuickTweenable* instigator) const
	{
		if (!Owner) return true; // No owner means it's not in a sequence
		return instigator == Owner;
	};
private:



	/** Array of tween groups in the sequence. */
	UPROPERTY(Transient)
	TArray<FQuickTweenSequenceGroup> TweenGroups;

	/** Elapsed time since the sequence started. */
	float ElapsedTime = 0.0f;

	/** Progress of the sequence (0.0 to 1.0). */
	float Progress = 0.0f;

	/** Whether the sequence is currently playing. */
	bool bIsPlaying = false;

	/** Whether the sequence has completed. */
	bool bIsCompleted = false;

	/** Whether the sequence is playing backwards. */
	bool bIsReversed = false;

	/** Whether the sequence is playing backwards internally. */
	bool bIsBackwards = false;

	/** Whether the sequence has started. */
	bool bHasStarted = false;

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

	/** If the sequence is waiting for remove. */
	bool bIsPendingKill = false;

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
