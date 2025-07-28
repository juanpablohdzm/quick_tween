// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TweenBuilders/QuickTweenBuilderObject.h"
#include "QuickTweenBuilderMaterial.generated.h"

enum class EEaseType : uint8;
/**
 * 
 */
UCLASS(BlueprintType)
class UQuickTweenBuilderMaterial : public UQuickTweenBuilderObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* VectorParameterTo(FName ParameterName, FVector To, float Duration, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* ScalarParameterTo(FName ParameterName, float To, float Duration, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* ColorParameterTo(FName ParameterName, FColor To, float Duration, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetLoops(int32 Loops);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetLoopType(ELoopType LoopType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetEaseType(EEaseType EaseType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetEaseCurve(UCurveFloat* EaseCurve);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetTimeScale(float TimeScale);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetIsBackwards(bool bIsBackwards);
};
