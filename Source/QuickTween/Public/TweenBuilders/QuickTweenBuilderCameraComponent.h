// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TweenBuilders/QuickTweenBuilderSceneComponent.h"
#include "Camera/CameraComponent.h"
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
	UQuickTweenBuilderCameraComponent* AspectRatioTo(float To, float Duration, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* FarClipPlaneTo(float To, float Duration, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* NearClipPlaneTo(float To, float Duration, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* MoveTo(FVector To, float Duration, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* RotateTo(FRotator To, float Duration, bool bUseShortestPath = true, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* RotateToQuat(FQuat To, float Duration, bool bUseShortestPath = true, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* LookAt(FVector Forward, float Duration, bool bUseShortestPath = true, FVector Up = FVector::UpVector, const FString& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetLoops(int32 Loops);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetLoopType(ELoopType LoopType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetEaseType(EEaseType EaseType);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetEaseCurve(UCurveFloat* EaseCurve);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetTimeScale(float TimeScale);

	UFUNCTION(BlueprintCallable)
	UQuickTweenBuilderCameraComponent* SetIsBackwards(bool bIsBackwards);
};
