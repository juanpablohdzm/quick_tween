// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenable.h"
#include "UObject/Object.h"
#include "../Utils/LoopType.h"
#include "QuickTweenSequence.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateTweenSequence, UQuickTweenSequence*, TweenSequence);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteTweenSequence, UQuickTweenSequence*, TweenSequence);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilledTweenSequence, UQuickTweenSequence*, TweenSequence);

class UQuickTweenBase;

/**
 * UQuickTweenSequence manages a sequence of tween animations.
 * Allows joining, appending, and controlling multiple tweens as a group.
 * Supports looping, reversing, and querying sequence state.
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickTweenSequence : public UObject, public IQuickTweenable
{
	GENERATED_BODY()

public:

#pragma region Sequence Creation
	/**
	 * Set up the sequence with optional looping parameters.
	 * @param worldContextObject Context object for world access (OPTIONAL ONLY FOR TESTS).
	 * @param loops Number of times to loop the sequence (-1 = infinite).
	 * @param loopType Type of looping behavior.
	 * @param id Optional identifier for the sequence.
	 * @return Reference to this sequence.
	 */
	UQuickTweenSequence* SetUp(
		const UObject* worldContextObject = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& id = FString());

	/**
	 * Joins a tween to the current group, allowing them to run in parallel.
	 * @param tween The tween to join.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Creation")
	UQuickTweenSequence* Join(UQuickTweenBase* tween);

	/**
	 * Appends a tween to the sequence, running after previous tweens complete.
	 * @param tween The tween to append.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Creation")
	UQuickTweenSequence* Append(UQuickTweenBase* tween);

#pragma endregion

#pragma region Sequence Control
public:
	/**
	 * Starts playing the sequence.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Control")
	UQuickTweenSequence* Play();

	/**
	 * Pauses the sequence.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Control")
	UQuickTweenSequence* Pause();

	/**
	 * Completes the sequence immediately.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Control")
	UQuickTweenSequence* Complete();

	/**
	 * Restarts the sequence from the beginning.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Control")
	UQuickTweenSequence* Restart();

	/**
	 * Kills the sequence and all its tweens.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Control")
	UQuickTweenSequence* KillSequence();

	/**
	 * Reverses the direction of the sequence.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Control")
	UQuickTweenSequence* Reverse();

private:
	/**
	 * Reverses all tweens in the sequence.
	 * @return Reference to this sequence.
	 */
	UQuickTweenSequence* Reverse_Tweens();
public:

	/**
	 * Toggles the pause state of the sequence.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Control")
	UQuickTweenSequence* TogglePause();

	/**
	 * Updates the sequence by the given delta time.
	 * @param deltaTime Time since last update.
	 */
	virtual void Update(float deltaTime) override;

private:
	/**
	 * Updates the sequence in Restart loop mode.
	 * @param deltaTime Time since last update.
	 */
	void Update_Restart(float deltaTime);

	/**
	 * Updates the sequence in PingPong loop mode.
	 * @param deltaTime Time since last update.
	 */
	void Update_PingPong(float deltaTime);
public:

	/**
	 * If this sequence should be eliminated from the manager.
	 * @return True if the sequence is pending kill, false otherwise.
	 */
	virtual bool GetIsPendingKill() const override { return bIsPendingKill; }
#pragma endregion


#pragma region Sequence State Queries
public:
	/**
	 * Checks if the sequence is currently playing.
	 * @return True if playing, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	[[nodiscard]] virtual bool GetIsPlaying() const override { return bIsPlaying;}

	/**
	 * Checks if the sequence has completed.
	 * @return True if completed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	[[nodiscard]] virtual bool GetIsCompleted() const override { return bIsCompleted; }

	/**
	 * Gets the total duration of the sequence.
	 * @return Duration in seconds.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	[[nodiscard]] float GetDuration() const;

	/**
	 * Gets the elapsed time since the sequence started.
	 * @return Elapsed time in seconds.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	[[nodiscard]] float GetElapsedTime() const { return ElapsedTime; }

	/**
	 * Gets the number of loops set for the sequence.
	 * @return Number of loops.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	[[nodiscard]] float GetLoops() const { return Loops; }

	/**
	 * Gets the current loop index.
	 * @return Current loop number.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	[[nodiscard]] float GetCurrentLoop() const { return CurrentLoop; }

	/**
	 * Gets the loop type of the sequence.
	 * @return The loop type.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	[[nodiscard]] ELoopType GetLoopType() const { return LoopType; }

	/**
	 * Gets the identifier of the sequence.
	 * @return Sequence ID.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	[[nodiscard]] FString GetId() const { return SequenceTweenId; }

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
	[[nodiscard]] UQuickTweenBase* GetTween(int32 index) const;
#pragma endregion

#pragma region Delegates
public:
	/** Called when the sequence updates. */
	UPROPERTY(BlueprintAssignable)
	FOnUpdateTweenSequence OnUpdate;

	/** Called when the sequence completes. */
	UPROPERTY(BlueprintAssignable)
	FOnCompleteTweenSequence OnComplete;

	/** Called when the sequence is killed. */
	UPROPERTY(BlueprintAssignable)
	FOnKilledTweenSequence OnKilled;
#pragma endregion

private:

	/**
	 * Represents a group of tweens that run in parallel.
	 */
	struct FQuickTweenSequenceGroup
	{
		TArray<TWeakObjectPtr<UQuickTweenBase>> Tweens;
	};

	/** Array of tween groups in the sequence. */
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

	bool bIsPendingKill = false;

	UPROPERTY()
	const UObject* WorldContextObject = nullptr;

};
