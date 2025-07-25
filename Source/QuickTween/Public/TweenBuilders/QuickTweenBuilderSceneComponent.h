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
USTRUCT(BlueprintType)
struct FQuickTweenBuilderSceneComponent : public FQuickTweenBuilderObject
{
	GENERATED_BODY()
public:
	FQuickTweenBuilderSceneComponent(USceneComponent* InTarget) : FQuickTweenBuilderObject(InTarget) {}

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderSceneComponent& MoveTo(FVector To, float Duration, FString&& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderSceneComponent& RotateTo(FRotator To, float Duration, bool bUseShortestPath = true, FString&& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderSceneComponent& RotateTo(FQuat To, float Duration, bool bUseShortestPath = true, FString&& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderSceneComponent& ScaleTo(FVector To, float Duration, FString&& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	FQuickTweenBuilderSceneComponent& LookAt(FVector Forward, float Duration, bool bUseShortestPath = true, FVector Up = FVector::UpVector, FString&& TweenTag = "");

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderSceneComponent& SetLoops(int32 Loops) override;

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderSceneComponent& SetLoopType(ELoopType LoopType) override;

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderSceneComponent& SetEaseType(EEaseType EaseType) override;

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderSceneComponent& SetEaseCurve(UCurveFloat* EaseCurve) override;

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderSceneComponent& SetTimeScale(float TimeScale) override;

	UFUNCTION(BlueprintCallable)
	virtual FQuickTweenBuilderSceneComponent& SetIsBackwards(bool bIsBackwards) override;

	UFUNCTION(BlueprintCallable)
	virtual UQuickTweenBase* Build() override;
};
