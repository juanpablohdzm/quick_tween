// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utils/EaseType.h"
#include "Utils/LoopType.h"
#include "QuickTweenLibrary.generated.h"

class UQuickVectorTween;
/**
 * 
 */
UCLASS()
class QUICKTWEEN_API UQuickTweenLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Sequence | Create | Make"), Category = "QuickTween")
	static UQuickTweenSequence* MakeQuickTweenSequence(
		UObject* worldContextObject,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Move To SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent"), Category = "QuickTween")
	static UQuickVectorTween* MoveTo_SceneComponent(
		UObject* worldContextObject,
		USceneComponent* component,
		FVector to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "",
		bool bShouldAutoKill = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Scale To SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent"), Category = "QuickTween")
	static UQuickVectorTween* ScaleTo_SceneComponent(
		UObject* worldContextObject,
		USceneComponent* component,
		FVector to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "",
		bool bShouldAutoKill = true);

	/*UFUNCTION(BlueprintCallable)
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
		FString tweenTag = "");*/
};
