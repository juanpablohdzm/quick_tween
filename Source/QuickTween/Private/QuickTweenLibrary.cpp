// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickTweenLibrary.h"

#include "TweenBuilders/QuickTweenBuilderCameraComponent.h"
#include "TweenBuilders/QuickTweenBuilderMaterial.h"
#include "TweenBuilders/QuickTweenBuilderSceneComponent.h"


UQuickTweenBuilderSceneComponent* UQuickTweenLibrary::CreateQuickTweenBuilderSceneComp(USceneComponent* InTarget)
{
	UQuickTweenBuilderSceneComponent* builder = NewObject<UQuickTweenBuilderSceneComponent>();
	builder->Initialize(InTarget);
	return builder;
}

UQuickTweenBuilderCameraComponent* UQuickTweenLibrary::CreateQuickTweenBuilderCameraComp(UCameraComponent* InTarget)
{
	UQuickTweenBuilderCameraComponent* builder = NewObject<UQuickTweenBuilderCameraComponent>();
	builder->Initialize(InTarget);
	return builder;
}

UQuickTweenBuilderMaterial* UQuickTweenLibrary::CreateQuickTweenBuilderMaterial(UMaterialInstanceDynamic* InTarget)
{
	UQuickTweenBuilderMaterial* builder = NewObject<UQuickTweenBuilderMaterial>();
	builder->Initialize(InTarget);
	return builder;
}