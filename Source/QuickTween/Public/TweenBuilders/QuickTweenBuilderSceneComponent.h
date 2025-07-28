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
	UQuickTweenBuilderSceneComponent* MoveTo(FVector To, float Duration, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* RotateTo(FRotator To, float Duration, bool bUseShortestPath = true, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* RotateToQuat(FQuat To, float Duration, bool bUseShortestPath = true, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* ScaleTo(FVector To, float Duration, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* LookAt(FVector Forward, float Duration, bool bUseShortestPath = true, FVector Up = FVector::UpVector, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetLoops(int32 Loops);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetLoopType(ELoopType LoopType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetEaseType(EEaseType EaseType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetEaseCurve(UCurveFloat* EaseCurve);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetTimeScale(float TimeScale);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* SetIsBackwards(bool bIsBackwards);
};
