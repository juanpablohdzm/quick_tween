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
	UQuickTweenBuilderMaterial* VectorParameterTo(FName parameterName, FVector To, float duration, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* ScalarParameterTo(FName parameterName, float To, float duration, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* ColorParameterTo(FName parameterName, FColor To, float duration, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetLoops(int32 loops);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetLoopType(ELoopType loopType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetEaseType(EEaseType easeType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetEaseCurve(UCurveFloat* easeCurve);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetTimeScale(float timeScale);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderMaterial* SetIsBackwards(bool bIsBackwards);
};
