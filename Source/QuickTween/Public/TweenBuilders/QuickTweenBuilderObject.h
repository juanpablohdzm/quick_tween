// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBuilderObject.generated.h"

class UQuickTweenSequence;
enum class EEaseType : uint8;
enum class ELoopType : uint8;
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
	UQuickTweenBuilderObject* SetLoopsBase(int32 loops);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetLoopTypeBase(ELoopType loopType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetEaseTypeBase(EEaseType easeType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetEaseCurveBase(UCurveFloat* easeCurve);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetTimeScaleBase(float timeScale);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetIsBackwardsBase(bool bIsBackwards);

	UFUNCTION(BlueprintCallable)
	virtual UQuickTweenSequence* Build() { return Sequence; }

protected:
	UPROPERTY(Transient)
	UQuickTweenSequence* Sequence;

	UPROPERTY(Transient)
	UObject* Target;

	bool bIsAppend = true;
};
