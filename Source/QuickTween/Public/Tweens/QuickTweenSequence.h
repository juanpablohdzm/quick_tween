// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenable.h"
#include "UObject/Object.h"
#include "../Utils/LoopType.h"
#include "QuickTweenSequence.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateTweenSequence, UObject*, TweenSequence);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteTweenSequence, UObject*, TweenSequence);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilledTweenSequence, UObject*, TweenSequence);

class UQuickTweenBase;

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
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickTweenSequence : public UQuickTweenable
{
	GENERATED_BODY()

public:
	virtual ~UQuickTweenSequence();

#pragma region Sequence Creation
	/**
	 * Set up the sequence with optional looping parameters.
	 * @param worldContextObject Context object for world access (OPTIONAL ONLY FOR TESTS).
	 * @param loops Number of times to loop the sequence (-1 = infinite).
	 * @param loopType Type of looping behavior.
	 * @param id Optional identifier for the sequence.
	 * @param bShouldAutoKill Whether to auto-kill the sequence on completion.
	 * @param bShouldPlayWhilePaused Whether the sequence should play while the game is paused.
	 * @return Reference to this sequence.
	 */
	void SetUp(
		const UObject* worldContextObject = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& id = FString(),
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false);

	/**
	 * Joins a tween to the current group, allowing them to run in parallel.
	 * @param tween The tween to join.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Creation")
	void Join(UQuickTweenable* tween);

	/**
	 * Appends a tween to the sequence, running after previous tweens complete.
	 * @param tween The tween to append.
	 * @return Reference to this sequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sequence"), Category = "Sequence|Creation")
	void Append(UQuickTweenable* tween);

	virtual void SetOwner(UQuickTweenable* owner) override { Owner = owner; }
#pragma endregion

#pragma region Sequence Control
public:


	virtual void Play(UQuickTweenable* instigator = nullptr) override;
	virtual void Pause(UQuickTweenable* instigator = nullptr) override;
	virtual void Stop(UQuickTweenable* instigator = nullptr) override;
	virtual void Reverse(UQuickTweenable* instigator = nullptr) override;
	virtual void Complete(UQuickTweenable* instigator = nullptr) override;
	virtual void Restart(UQuickTweenable* instigator = nullptr) override;
	virtual void Reset(UQuickTweenable* instigator = nullptr) override;
	virtual void Kill(UQuickTweenable* instigator = nullptr) override;
	virtual void Update(float deltaTime, UQuickTweenable* instigator = nullptr) override;
	virtual void SetAutoKill(bool bShouldAutoKill, UQuickTweenable* instigator = nullptr) override;

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

	/**
	 * If this sequence should be eliminated from the manager.
	 * @return True if the sequence is pending kill, false otherwise.
	 */
	virtual bool GetIsPendingKill() const override { return bIsPendingKill; }
#pragma endregion


#pragma region Sequence State Queries
public:


	[[nodiscard]] virtual bool GetIsPlaying() const override { return bIsPlaying;}
	[[nodiscard]] virtual float GetProgress() const override { return Progress; }
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
