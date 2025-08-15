#pragma once

#include "CoreMinimal.h"
#include "../Utils/EaseType.h"
#include "../Utils/LoopType.h"
#include "QuickTweenBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartTween, UQuickTweenBase*, Tween);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateTween, UQuickTweenBase*, Tween, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteTween, UQuickTweenBase*, Tween);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilledTween, UQuickTweenBase*, Tween);

UCLASS(Blueprintable, BlueprintType, Abstract)
class QUICKTWEEN_API UQuickTweenBase : public UObject
{
	GENERATED_BODY()
public:

	void Initialize(
		float duration,
		float timeScale,
		EEaseType easeType,
		UCurveFloat* easeCurve,
		int32 loops,
		ELoopType loopType,
		const FString& tweenTag = FString());

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	virtual bool Update(float deltaTime);

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	virtual UQuickTweenBase* Play();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	virtual UQuickTweenBase* Pause();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	virtual UQuickTweenBase* Stop();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	virtual UQuickTweenBase* Reverse();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	virtual UQuickTweenBase* Restart();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	virtual UQuickTweenBase* Complete();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	virtual UQuickTweenBase* Reset();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	virtual void Kill();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] float GetDuration() const { return Duration;}
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	UQuickTweenBase* SetDuration(float duration) { Duration = duration; return this; }

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] float GetProgress() const { return Progress; }
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	UQuickTweenBase* SetProgress(float progress) { Progress = progress; return this; }

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] float GetTimeScale() const { return TimeScale; }
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	UQuickTweenBase* SetTimeScale(float timeScale) { TimeScale = timeScale; return this; }

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] bool GetIsPlaying() const { return bIsPlaying; }
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	UQuickTweenBase* SetIsPlaying(bool isPlaying) { bIsPlaying = isPlaying; return this; }

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] bool GetIsCompleted() const { return bIsCompleted; }
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	UQuickTweenBase* SetIsCompleted(bool isCompleted) { bIsCompleted = isCompleted; return this; }

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] bool GetIsBackwards() const { return bIsBackwards; }
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	UQuickTweenBase* SetIsBackwards(bool isBackwards) { bIsBackwards = isBackwards; return this; }

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] EEaseType GetEaseType() const { return EaseType; }
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	UQuickTweenBase* SetEaseType(EEaseType easeType) { EaseType = easeType; return this;}

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] UCurveFloat* GetEaseCurve() const { return EaseCurve; }
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	UQuickTweenBase* SetEaseCurve(UCurveFloat* easeCurve) { EaseCurve = easeCurve; return this; }

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] int32 GetLoops() const { return Loops; }
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	UQuickTweenBase* SetLoops(int32 loops) { Loops = loops; return this;}

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] ELoopType GetLoopType() const { return LoopType; }
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	UQuickTweenBase* SetLoopType(ELoopType loopType){ LoopType = loopType; return this;}

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	[[nodiscard]] FString GetTweenTag() const { return TweenTag; }
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "Tween|Info")
	UQuickTweenBase* SetTweenTag(const FString& tag) { TweenTag = tag; return this; }

protected:

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentLoop() const { return CurrentLoop; }

	UPROPERTY(BlueprintAssignable)
	FOnStartTween OnStart;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateTween OnUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnCompleteTween OnComplete;

	UPROPERTY(BlueprintAssignable)
	FOnKilledTween OnKilled;

	float ElapsedTime = 0.0f;

private:
	float Duration = 0.0f;
	float Progress = 0.0f;
	float TimeScale = 1.0f;

	bool bIsPlaying = false;
	bool bIsCompleted = false;
	bool bIsBackwards = false;

	EEaseType EaseType = EEaseType::Linear;
	UPROPERTY(Transient)
	UCurveFloat* EaseCurve = nullptr;

	int32 CurrentLoop = 1;
	int32 Loops = 0;
	ELoopType LoopType = ELoopType::Restart;

	FString TweenTag;
};