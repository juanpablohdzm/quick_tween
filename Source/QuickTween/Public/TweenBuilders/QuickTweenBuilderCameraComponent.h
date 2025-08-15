// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TweenBuilders/QuickTweenBuilderSceneComponent.h"
#include "QuickTweenBuilderCameraComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UQuickTweenBuilderCameraComponent : public UQuickTweenBuilderObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* AspectRatioTo(float to, float duration, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* FarClipPlaneTo(float to, float duration, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* NearClipPlaneTo(float to, float duration, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* MoveTo(FVector to, float duration, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* RotateTo(FRotator to, float duration, bool bUseShortestPath = true, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* RotateToQuat(FQuat to, float duration, bool bUseShortestPath = true, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* LookAt(FVector Forward, float duration, bool bUseShortestPath = true, FVector up = FVector::UpVector, const FString& tweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetLoops(int32 loops);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetLoopType(ELoopType loopType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetEaseType(EEaseType easeType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetEaseCurve(UCurveFloat* easeCurve);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetTimeScale(float timeScale);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetIsBackwards(bool bIsBackwards);
};
