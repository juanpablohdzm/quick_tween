// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBuilderObject.generated.h"

class UQuickTweenBase;
/**
 * 
 */
USTRUCT()
struct FQuickTweenBuilderObject
{
	GENERATED_BODY()
public:
	FQuickTweenBuilderObject(UObject* InObject) : Target(InObject) {}
	virtual ~FQuickTweenBuilderObject() = default;

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderObject& SetLoops(int32 Loops);

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderObject& SetLoopType(ELoopType LoopType);

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderObject& SetEaseType(EEaseType EaseType);

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderObject& SetEaseCurve(UCurveFloat* EaseCurve);

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderObject& SetTimeScale(float TimeScale);

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderObject& SetIsBackwards(bool bIsBackwards);

	UFUNCTION(BlueprintCallable)
	virtual UQuickTweenBase* Build() { return nullptr; }

protected:
	UPROPERTY(Transient)
	TArray<UQuickTweenBase*> Tweens;

	UPROPERTY(Transient)
	UObject* Target;
};
