// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickTweenLibrary.h"

#include "TweenBuilders/QuickTweenBuilderCameraComponent.h"
#include "TweenBuilders/QuickTweenBuilderMaterial.h"
#include "TweenBuilders/QuickTweenBuilderSceneComponent.h"


UQuickTweenBuilderSceneComponent* UQuickTweenLibrary::CreateQuickTweenBuilderSceneComp(USceneComponent* inTarget)
{
	UQuickTweenBuilderSceneComponent* builder = NewObject<UQuickTweenBuilderSceneComponent>();
	builder->Initialize(inTarget);
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