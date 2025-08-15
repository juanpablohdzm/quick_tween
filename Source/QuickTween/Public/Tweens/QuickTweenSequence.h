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
 * 
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickTweenSequence : public UObject
{
	GENERATED_BODY()

public:

#pragma region Sequence Creation
	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Join(UQuickTweenBase* tween);

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Append(UQuickTweenBase* tween);

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* SetLoops(int32 loops = 1);

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* SetLoopType(ELoopType loopType = ELoopType::Restart);

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* SetId(const FString& id);
#pragma endregion

#pragma region Sequence Control
	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Play();

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Pause();

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Complete();

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Restart();

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* KillSequence();

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Reverse();

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* TogglePause();

	void Update(float deltaTime);
#pragma endregion


#pragma region Sequence State Queries
	UFUNCTION(BlueprintCallable)
	bool GetIsPlaying() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsCompleted() const;

	UFUNCTION(BlueprintCallable)
	float GetDuration() const;

	UFUNCTION(BlueprintCallable)
	float GetElapsedTime() const;

	UFUNCTION(BlueprintCallable)
	float GetLoops() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentLoop();


	UFUNCTION(BlueprintCallable)
	ELoopType GetLoopType() const;

	UFUNCTION(BlueprintCallable)
	FString GetId() const;

	UFUNCTION(BlueprintCallable)
	int32 GetNumTweens() const;

	UFUNCTION(BlueprintCallable)
	UQuickTweenBase* GetTween(int32 index) const;
#pragma endregion

#pragma region Delegates
	UPROPERTY(BlueprintAssignable)
	FOnStartTweenSequence OnStart;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateTweenSequence OnUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnCompleteTweenSequence OnComplete;

	UPROPERTY(BlueprintAssignable)
	FOnKilledTweenSequence OnKilled;
#pragma endregion

private:

	struct FQuickTweenSequenceGroup
	{
		TArray<TWeakObjectPtr<UQuickTweenBase>> Tweens;
	};


	TArray<FQuickTweenSequenceGroup> TweenGroups;

	float ElapsedTime = 0.0f;
	float Progress = 0.0f;

	bool bIsPlaying = false;
	bool bIsCompleted = false;
	bool bIsBackwards = false;

	int32 CurrentLoop = 1;
	int32 Loops = 0;
	ELoopType LoopType = ELoopType::Restart;

	FString SequenceTweenId;

	int32 CurrentTweenGroupIndex = 0;

};
