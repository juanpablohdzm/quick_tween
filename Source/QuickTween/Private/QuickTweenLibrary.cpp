// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickTweenLibrary.h"

#include "Camera/CameraComponent.h"
#include "TweenBuilders/QuickTweenBuilderCameraComponent.h"
#include "TweenBuilders/QuickTweenBuilderMaterial.h"
#include "TweenBuilders/QuickTweenBuilderSceneComponent.h"


UQuickTweenBuilderSceneComponent* UQuickTweenLibrary::CreateQuickTweenBuilderSceneComp(
	USceneComponent* inTarget,
	UObject* worldContextObject,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag)
{
	UQuickTweenBuilderSceneComponent* builder = NewObject<UQuickTweenBuilderSceneComponent>();
	builder->Initialize(inTarget);
	builder->SetUp(worldContextObject, loops, loopType, tweenTag);
	return builder;
}

UQuickTweenBuilderCameraComponent* UQuickTweenLibrary::CreateQuickTweenBuilderCameraComp(UCameraComponent* inTarget)
{
	UQuickTweenBuilderCameraComponent* builder = NewObject<UQuickTweenBuilderCameraComponent>();
	builder->Initialize(inTarget);
	return builder;
}

UQuickTweenBuilderMaterial* UQuickTweenLibrary::CreateQuickTweenBuilderMaterial(UMaterialInstanceDynamic* inTarget)
{
	UQuickTweenBuilderMaterial* builder = NewObject<UQuickTweenBuilderMaterial>();
	builder->Initialize(inTarget);
	return builder;
}