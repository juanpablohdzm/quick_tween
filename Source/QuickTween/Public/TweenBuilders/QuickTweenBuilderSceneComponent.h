// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TweenBuilders/QuickTweenBuilderObject.h"
#include "QuickTweenBuilderSceneComponent.generated.h"

enum class EEaseType : uint8;
enum class ELoopType : uint8;
/**
 * 
 */
UCLASS(BlueprintType)
class UQuickTweenBuilderSceneComponent : public UQuickTweenBuilderObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* MoveTo(FVector to, float duration, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* RotateTo(FRotator to, float duration, bool bUseShortestPath = true, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* RotateToQuat(FQuat to, float duration, bool bUseShortestPath = true, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* ScaleTo(FVector to, float duration, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* LookAt(FVector forward, float duration, bool bUseShortestPath = true, FVector up = FVector::UpVector, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetLoops(int32 loops);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetLoopType(ELoopType loopType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetEaseType(EEaseType easeType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetEaseCurve(UCurveFloat* easeCurve);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetTimeScale(float timeScale);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetIsBackwards(bool bIsBackwards);
};
