// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBuilderObject.generated.h"

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
	virtual ~UQuickTweenBuilderObject() = default;
	virtual void Initialize(UObject* InObject) { Target = InObject; }

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetLoopsBase(int32 Loops);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetLoopTypeBase(ELoopType LoopType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetEaseTypeBase(EEaseType EaseType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetEaseCurveBase(UCurveFloat* EaseCurve);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetTimeScaleBase(float TimeScale);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderObject* SetIsBackwardsBase(bool bIsBackwards);

	UFUNCTION(BlueprintCallable)
	virtual UQuickTweenBase* Build() { return nullptr; }

protected:
	UPROPERTY(Transient)
	TArray<UQuickTweenBase*> Tweens;

	UPROPERTY(Transient)
	UObject* Target;
};
