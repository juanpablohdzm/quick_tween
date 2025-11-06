#pragma once

#include "CoreMinimal.h"
#include "QuickTweenable.h"
#include "../Utils/EaseType.h"
#include "../Utils/LoopType.h"
#include "QuickTweenBase.generated.h"

class UQuickTweenSequence;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartTween, UObject*, Tween);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateTween, UObject*, Tween, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteTween, UObject*, Tween);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilledTween, UObject*, Tween);

/**
 * Base class for all QuickTween tweens.
 * Provides core tweening functionality, state, and events.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class QUICKTWEEN_API UQuickTweenBase : public UObject, public IQuickTweenable
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
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false);

	/** Sets whether this tween is part of a sequence. */
	virtual void SetOwner(UObject* owner) override { Owner = owner; }

protected:
	/** Sets the progress of the tween (0-1). */
	void SetProgress(float progress) { Progress = progress; }

#pragma endregion

#pragma region Tween Control
public:

	virtual void Play(UObject* instigator = nullptr) override;

	virtual void Pause(UObject* instigator = nullptr) override;

	virtual void Stop(UObject* instigator = nullptr) override;

	virtual void Reverse(UObject* instigator = nullptr) override;

	virtual void Restart(UObject* instigator = nullptr) override;

	virtual void Complete(UObject* instigator = nullptr) override;

	virtual void Reset(UObject* instigator = nullptr) override;

	virtual void Kill(UObject* instigator = nullptr) override;

	virtual void Update(float deltaTime, UObject* instigator = nullptr) override;

	virtual void SetAutoKill(bool bShouldAutoKill, UObject* instigator = nullptr) override;
private:

	void Update_Restart(float deltaTime, UObject* instigator);


	void Update_PingPong(float deltaTime, UObject* instigator);

#pragma endregion

#pragma region Tween State Queries
public:
	/**
	 * If this tween should be eliminated from the manager.
	 * @return True if the tween is pending kill, false otherwise.
	 */
	virtual bool GetIsPendingKill() const override { return bIsPendingKill; }

	/** Gets the duration of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual float GetDuration() const override { return Duration;}

	/** Gets the current progress of the tween (0-1). */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual float GetProgress() const override { return Progress; }

	[[nodiscard]] virtual float GetElapsedTime() const override { return ElapsedTime; }

	/** Gets the time scale of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual float GetTimeScale() const override { return TimeScale; }

	/** Returns true if the tween is currently playing. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsPlaying() const override { return bIsPlaying; }

	/** Returns true if the tween is completed. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsCompleted() const override { return bIsCompleted; }

	/** Returns true if the tween is playing backwards. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsBackwards() const override { return bIsBackwards; }

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual bool GetIsReversed() const override { return bIsReversed; }

	/** Gets the ease type of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual EEaseType GetEaseType() const override { return EaseType; }

	/** Gets the custom ease curve of the tween, if any. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual UCurveFloat* GetEaseCurve() const override { return EaseCurve; }

	/** Gets the number of loops for the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual int32 GetLoops() const override{ return Loops; }

	/** Gets the loop type of the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual ELoopType GetLoopType() const override { return LoopType; }

	/** Gets the tag assigned to the tween. */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] virtual FString GetTweenTag() const override { return TweenTag; }

	/** Gets the current loop index (1-based). */
	UFUNCTION(BlueprintCallable)
	[[nodiscard]] virtual int32 GetCurrentLoop() const override { return CurrentLoop; }

	/** Gets whether this tween will be removed after completion. */
	UFUNCTION(BlueprintCallable)
	[[nodiscard]] virtual bool GetAutoKill() const override { return bAutoKill; }

	/** Gets whether this tween should play while paused. */
	UFUNCTION(BlueprintCallable)
	[[nodiscard]] virtual bool GetShouldPlayWhilePaused() const override {return bPlayWhilePaused;}
#pragma endregion

protected:
	bool InstigatorIsOwner(UObject* instigator) const
	{
		if (!Owner) return true; // No owner means it's not in a sequence
		return instigator == Owner;
	};

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

	/** If this tween has an owner */
	UPROPERTY()
	UObject* Owner = nullptr;

	/** If this tween should be eliminated from the manager. */
	bool bIsPendingKill = false;

	/** If the tween should auto-kill upon completion. */
	bool bAutoKill = true;

	/** If the tween should play while the game is paused. */
	bool bPlayWhilePaused = false;

	UPROPERTY()
	const UObject* WorldContextObject = nullptr;
};