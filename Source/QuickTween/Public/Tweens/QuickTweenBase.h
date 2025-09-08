#pragma once

#include "CoreMinimal.h"
#include "QuickTweenable.h"
#include "../Utils/EaseType.h"
#include "../Utils/LoopType.h"
#include "Utils/Badge.h"
#include "QuickTweenBase.generated.h"

class UQuickTweenSequence;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartTween, UQuickTweenBase*, Tween);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateTween, UQuickTweenBase*, Tween, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteTween, UQuickTweenBase*, Tween);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilledTween, UQuickTweenBase*, Tween);

/**
 * Base class for all QuickTween tweens.
 * Provides core tweening functionality, state, and events.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class QUICKTWEEN_API UQuickTweenBase : public UObject, public IQuickTweenable
{
	GENERATED_BODY()
public:
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
	 */
	void SetUp(
		float duration,
		float timeScale,
		EEaseType easeType,
		UCurveFloat* easeCurve,
		int32 loops,
		ELoopType loopType,
		const FString& tweenTag = FString(),
		const UObject* worldContextObject = nullptr);

	/** Sets whether this tween is part of a sequence. */
	UQuickTweenBase* SetIsInSequence(Badge<UQuickTweenSequence> badge, bool bInSequence) { bIsInSequence = bInSequence; return this; }

protected:
	/** Sets the progress of the tween (0-1). */
	UQuickTweenBase* SetProgress(float progress) { Progress = progress; return this; }

#pragma endregion

#pragma region Tween Control
public:
	/**
	 * Starts or resumes the tween.
	 * @return This tween instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Control")
	virtual UQuickTweenBase* Play();

	/**
	 * Starts or resumes the tween from a sequence.
	 * @param badge Badge pointer to allow internal calls from sequences.
	 * @return This tween instance.
	 */
	virtual UQuickTweenBase* Play(Badge<UQuickTweenSequence>* badge);

	/**
	 * Pauses the tween.
	 * @return This tween instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Control")
	virtual UQuickTweenBase* Pause();

	/**
	 *
	 * Pauses the tween from a sequence.
	 * @param badge Bagde pointer to allow internal calls from sequences.
	 * @return This tween instance.
	 */
	virtual UQuickTweenBase* Pause(Badge<UQuickTweenSequence>* badge);

	/**
	 * Stops the tween and resets its progress.
	 * @return This tween instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Control")
	virtual UQuickTweenBase* Stop();

	/**
	 * Stops the tween from a sequence and resets its progress.
	 * @param badge Badge pointer to allow internal calls from sequences.
	 * @return This tween instance.
	 */
	virtual UQuickTweenBase* Stop(Badge<UQuickTweenSequence>* badge);

	/**
	 * Reverses the direction of the tween.
	 * @return This tween instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Control")
	virtual UQuickTweenBase* Reverse();

	/**
	 * Reverses the direction of the tween from a sequence.
	 * @param badge
	 * @return
	 */
	virtual UQuickTweenBase* Reverse(Badge<UQuickTweenSequence>* badge);

	/**
	 * Restarts the tween from the beginning.
	 * @return This tween instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Control")
	virtual UQuickTweenBase* Restart();

	/**
	 *
	 * Restarts the tween from a sequence.
	 * @param badge Badge pointer to allow internal calls from sequences.
	 * @return This tween instance.
	 */
	virtual UQuickTweenBase* Restart(Badge<UQuickTweenSequence>* badge);

	/**
	 * Completes the tween immediately.
	 * @return This tween instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Control")
	virtual UQuickTweenBase* Complete();

	/**
	 * Completes the tween immediately from a sequence.
	 * @param badge Badge pointer to allow internal calls from sequences.
	 * @return This tween instance.
	 */
	virtual UQuickTweenBase* Complete(Badge<UQuickTweenSequence>* badge);

	/**
	 * Resets the tween to its initial state.
	 * @return This tween instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Control")
	virtual UQuickTweenBase* Reset();


	/**
	 * Resets the tween to its initial state from a sequence.
	 * @param badge Badge pointer to allow internal calls from sequences.
	 * @return This tween instance.
	 */
	virtual UQuickTweenBase* Reset(Badge<UQuickTweenSequence>* badge);

	/**
	 * Kills the tween, stopping all updates and events.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Control")
	virtual void Kill();

	/**
	 * Kills the tween from a sequence, stopping all updates and events.
	 * @param badge Badge pointer to allow internal calls from sequences.
	 */
	virtual void Kill(Badge<UQuickTweenSequence>* badge);

	/**
	 * Updates the tween by the given delta time.
	 * @param deltaTime Time since last update.
	 * @return True if the tween is still active, false if completed or killed.
	 */
	virtual void Update(float deltaTime) override;

private:
	/**
	 * Updates the tween in Restart loop mode.
	 * @param deltaTime Time since last update.
	 * @param badge Badge pointer to allow internal calls from sequences.
	 */
	void Update_Restart(float deltaTime, Badge<UQuickTweenSequence>* badge);

	/**
	 * Updates the tween in PingPong loop mode.
	 * @param deltaTime Time since last update.
	 * @param badge Badge pointer to allow internal calls from sequences.
	 */
	void Update_PingPong(float deltaTime, Badge<UQuickTweenSequence>* badge);

public:
	/**
	 * Updates the tween by the given delta time from a sequence.
	 * @param deltaTime Time since last update.
	 * @param badge Badge pointer to allow internal calls from sequences.
	 */
	virtual void Update(float deltaTime, Badge<UQuickTweenSequence>* badge);

	/**
	 * If this tween should be eliminated from the manager.
	 * @return True if the tween is pending kill, false otherwise.
	 */
	virtual bool GetIsPendingKill() const override { return bIsPendingKill; }
#pragma endregion

#pragma region Tween State Queries
public:
	/** Gets the duration of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] float GetDuration() const { return Duration;}

	/** Gets the current progress of the tween (0-1). */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] float GetProgress() const { return Progress; }

	/** Gets the time scale of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] float GetTimeScale() const { return TimeScale; }

	/** Returns true if the tween is currently playing. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsPlaying() const override { return bIsPlaying; }

	/** Returns true if the tween is completed. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsCompleted() const override { return bIsCompleted; }

	/** Returns true if the tween is playing backwards. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] bool GetIsBackwards() const { return bIsBackwards; }

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] bool GetIsReversed() const { return bIsReversed; }

	/** Gets the ease type of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] EEaseType GetEaseType() const { return EaseType; }

	/** Gets the custom ease curve of the tween, if any. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] UCurveFloat* GetEaseCurve() const { return EaseCurve; }

	/** Gets the number of loops for the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] int32 GetLoops() const { return Loops; }

	/** Gets the loop type of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] ELoopType GetLoopType() const { return LoopType; }

	/** Gets the tag assigned to the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FString GetTweenTag() const { return TweenTag; }

	/** Gets the current loop index (1-based). */
	UFUNCTION(BlueprintCallable)
	[[nodiscard]] int32 GetCurrentLoop() const { return CurrentLoop; }
#pragma endregion

protected:

	/** Event triggered when the tween starts. */
	UPROPERTY(BlueprintAssignable)
	FOnStartTween OnStart;

	/** Event triggered when the tween updates. */
	UPROPERTY(BlueprintAssignable)
	FOnUpdateTween OnUpdate;

	/** Event triggered when the tween completes. */
	UPROPERTY(BlueprintAssignable)
	FOnCompleteTween OnComplete;

	/** Event triggered when the tween is killed. */
	UPROPERTY(BlueprintAssignable)
	FOnKilledTween OnKilled;

	/** Time elapsed since the tween started. */
	float ElapsedTime = 0.0f;

private:
	/** Duration of the tween in seconds. */
	float Duration = 0.0f;
	/** Current progress of the tween (0-1). */
	float Progress = 0.0f;
	/** Time scale multiplier. */
	float TimeScale = 1.0f;

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

	/** If this tween is part of a sequence */
	bool bIsInSequence = false;

	/** If this tween should be eliminated from the manager. */
	bool bIsPendingKill = false;
};