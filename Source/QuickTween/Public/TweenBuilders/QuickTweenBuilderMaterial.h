// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TweenBuilders/QuickTweenBuilderObject.h"
#include "QuickTweenBuilderMaterial.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FQuickTweenBuilderMaterial : public FQuickTweenBuilderObject
{
	GENERATED_BODY()
public:
	FQuickTweenBuilderMaterial(UMaterialInstanceDynamic* InTarget) : FQuickTweenBuilderObject(InTarget) {}

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderMaterial& VectorParameterTo(FName ParameterName, FVector To, float Duration, FString&& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderMaterial& ScalarParameterTo(FName ParameterName, float To, float Duration, FString&& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderMaterial& ColorParameterTo(FName ParameterName, FColor To, float Duration, FString&& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderMaterial& SetLoops(int32 Loops);

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderMaterial& SetLoopType(ELoopType LoopType);

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderMaterial& SetEaseType(EEaseType EaseType);

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderMaterial& SetEaseCurve(UCurveFloat* EaseCurve);

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderMaterial& SetTimeScale(float TimeScale);

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderMaterial& SetIsBackwards(bool bIsBackwards);

	UFUNCTION(BlueprintCallable)
	virtual UQuickTweenBase* Build() override;
};
