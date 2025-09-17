// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tweens/QuickTweenSequence.h"
#include "QuickTweenBuilderObject.generated.h"

class UQuickTweenSequence;
class UQuickTweenBase;
/**
 * 
 */
UCLASS()
class UQuickTweenBuilderObject : public UObject
{
	GENERATED_BODY()
public:
	virtual void Initialize(UObject* inObject);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetUp(
		const UObject* worldContextObject,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* AppendBase();

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* JoinBase();

	UFUNCTION(BlueprintCallable)
	virtual UQuickTweenSequence* Build()
	{
		Sequence->Play();
		return Sequence;
	}

protected:
	UPROPERTY(Transient)
	UQuickTweenSequence* Sequence;

	UPROPERTY(Transient)
	UObject* Target;

	bool bIsAppend = true;
};
