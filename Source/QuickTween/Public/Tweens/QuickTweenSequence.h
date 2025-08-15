// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuickTweenSequence.generated.h"


class UQuickTweenBase;
/**
 * 
 */
UCLASS(BlueprintType)
class QUICKTWEEN_API UQuickTweenSequence : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Play();

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Stop();

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Pause();

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Complete();

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Join(UQuickTweenBase* tween);

	UFUNCTION(BlueprintCallable)
	UQuickTweenSequence* Append(UQuickTweenBase* tween);

	UFUNCTION(BlueprintCallable)
	int32 GetNumTweens() const;

	UFUNCTION(BlueprintCallable)
	UQuickTweenBase* GetTween(int32 index) const;


	void Update(float deltaTime);

private:


	struct FQuickTweenSequenceGroup
	{
		TArray<TWeakObjectPtr<UQuickTweenBase>> Tweens;
	};


	TArray<FQuickTweenSequenceGroup> TweenGroups;


	int32 currentTweenIndex = 0;
	bool bIsPlaying = false;
	bool bIsCompleted = false;
	bool bIsBackwards = false;
};
