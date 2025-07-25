// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickTweenLibrary.h"

#include "TweenBuilders/QuickTweenBuilderCameraComponent.h"
#include "TweenBuilders/QuickTweenBuilderMaterial.h"
#include "TweenBuilders/QuickTweenBuilderSceneComponent.h"


FQuickTweenBuilderSceneComponent UQuickTweenLibrary::CreateQuickTweenBuilder(USceneComponent* InTarget)
{
	return FQuickTweenBuilderSceneComponent(InTarget);
}

FQuickTweenBuilderCameraComponent UQuickTweenLibrary::CreateQuickTweenBuilder(UCameraComponent* InTarget)
{
	return FQuickTweenBuilderCameraComponent(InTarget);
}

FQuickTweenBuilderMaterial UQuickTweenLibrary::CreateQuickTweenBuilder(UMaterialInstanceDynamic* InTarget)
{
	return FQuickTweenBuilderMaterial(InTarget);
}