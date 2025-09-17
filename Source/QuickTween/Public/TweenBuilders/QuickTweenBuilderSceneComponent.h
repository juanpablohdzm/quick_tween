// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TweenBuilders/QuickTweenBuilderObject.h"
#include "Utils/EaseType.h"
#include "QuickTweenBuilderSceneComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UQuickTweenBuilderSceneComponent : public UQuickTweenBuilderObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* MoveTo(
		FVector to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* RotateTo(
		FRotator to,
		bool bUseShortestPath = true,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* RotateToQuat(
		FQuat to,
		bool bUseShortestPath = true,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* ScaleTo(
		FVector to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* LookAt(
		FVector forward,
		bool bUseShortestPath = true,
		FVector up = FVector::UpVector,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* Append();

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderSceneComponent* Join();
};
