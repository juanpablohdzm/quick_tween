// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuickTweenLibrary.generated.h"

/**
 * 
 */
UCLASS()
class QUICKTWEEN_API UQuickTweenLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, ForceInline, meta = (Keywords = "Tween"), Category = "QuickTween")
	static FQuickTweenBuilderSceneComponent CreateQuickTweenBuilder(USceneComponent* InTarget);
	UFUNCTION(BlueprintCallable, ForceInline, meta = (Keywords = "Tween"), Category = "QuickTween")
	static FQuickTweenBuilderCameraComponent CreateQuickTweenBuilder(UCameraComponent* InTarget);
	UFUNCTION(BlueprintCallable, ForceInline, meta = (Keywords = "Tween"), Category = "QuickTween")
	static FQuickTweenBuilderMaterial CreateQuickTweenBuilder(UMaterialInstanceDynamic* InTarget);


};
