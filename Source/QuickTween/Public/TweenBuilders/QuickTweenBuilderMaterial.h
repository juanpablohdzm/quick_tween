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

};
