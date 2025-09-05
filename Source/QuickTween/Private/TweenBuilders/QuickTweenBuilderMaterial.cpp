// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderMaterial.h"


UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::VectorParameterTo(FName parameterName, FVector to,
                                                                          float duration, const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::ScalarParameterTo(FName parameterName, float to, float duration,
	const FString& tweenTag)
{
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::ColorParameterTo(FName parameterName, FColor to, float duration,
	const FString& tweenTag)
{
	return this;
}
