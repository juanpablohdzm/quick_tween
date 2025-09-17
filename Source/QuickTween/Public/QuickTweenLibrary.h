// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TweenBuilders/QuickTweenBuilderCameraComponent.h"
#include "TweenBuilders/QuickTweenBuilderMaterial.h"
#include "QuickTweenLibrary.generated.h"

/**
 * 
 */
UCLASS()
class QUICKTWEEN_API UQuickTweenLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "QuickTween")
	static UQuickTweenBuilderSceneComponent* CreateQuickTweenBuilderSceneComp(
		USceneComponent* InTarget,
		UObject* worldContextObject,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true);

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "QuickTween")
	static UQuickTweenBuilderCameraComponent* CreateQuickTweenBuilderCameraComp(class UCameraComponent* InTarget);
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween"), Category = "QuickTween")
	static UQuickTweenBuilderMaterial* CreateQuickTweenBuilderMaterial(UMaterialInstanceDynamic* InTarget);


};
