// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../Utils/LoopType.h"
#include "QuickTweenSequence.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartTweenSequence, UQuickTweenSequence*, TweenSequence);
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
class QUICKTWEEN_API UQuickTweenSequence : public UObject
{
	GENERATED_BODY()

public:

#pragma region Sequence Creation
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

	/**
	 * Sets the number of loops for the sequence.
	 * @param loops Number of loops (default is 1).
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Creation")
	UQuickTweenSequence* SetLoops(int32 loops = 1) { Loops = loops; return this; };

	/**
	 * Sets the loop type for the sequence.
	 * @param loopType The loop type (default is Restart).
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Creation")
	UQuickTweenSequence* SetLoopType(ELoopType loopType = ELoopType::Restart) { LoopType = loopType; return this; };

	/**
	 * Sets an identifier for the sequence.
	 * @param id The sequence ID.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Creation")
	UQuickTweenSequence* SetId(const FString& id) { SequenceTweenId = id; return this; };
#pragma endregion

#pragma region Sequence Control
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
	void Update(float deltaTime);
#pragma endregion


#pragma region Sequence State Queries
	/**
	 * Checks if the sequence is currently playing.
	 * @return True if playing, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	[[nodiscard]] bool GetIsPlaying() const { return bIsPlaying;}

	/**
	 * Checks if the sequence has completed.
	 * @return True if completed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	bool GetIsCompleted() const { return bIsCompleted; }

	/**
	 * Gets the total duration of the sequence.
	 * @return Duration in seconds.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	float GetDuration() const;

	/**
	 * Gets the elapsed time since the sequence started.
	 * @return Elapsed time in seconds.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	float GetElapsedTime() const { return ElapsedTime; }

	/**
	 * Gets the number of loops set for the sequence.
	 * @return Number of loops.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	float GetLoops() const { return Loops; }

	/**
	 * Gets the current loop index.
	 * @return Current loop number.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	float GetCurrentLoop() const { return CurrentLoop; }

	/**
	 * Gets the loop type of the sequence.
	 * @return The loop type.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	ELoopType GetLoopType() const { return LoopType; }

	/**
	 * Gets the identifier of the sequence.
	 * @return Sequence ID.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	FString GetId() const { return SequenceTweenId; }

	/**
	 * Gets the number of tweens in the sequence.
	 * @return Number of tweens.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	int32 GetNumTweens() const;

	/**
	 * Gets the tween at the specified index.
	 * @param index Index of the tween.
	 * @return Pointer to the tween.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|State")
	UQuickTweenBase* GetTween(int32 index) const;
#pragma endregion

#pragma region Delegates
	/** Called when the sequence starts. */
	UPROPERTY(BlueprintAssignable)
	FOnStartTweenSequence OnStart;

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
	bool bIsBackwards = false;

	/** Current loop index. */
	int32 CurrentLoop = 1;

	/** Total number of loops. */
	int32 Loops = 0;

	/** Loop type for the sequence. */
	ELoopType LoopType = ELoopType::Restart;

	/** Identifier for the sequence. */
	FString SequenceTweenId;

	/** Index of the current tween group. */
	int32 CurrentTweenGroupIndex = 0;

};
