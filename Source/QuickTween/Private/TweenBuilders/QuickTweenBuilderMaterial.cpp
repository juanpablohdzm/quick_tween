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

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetLoops(int32 loops)
{
	UQuickTweenBuilderObject::SetLoopsBase(loops);
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetLoopType(ELoopType loopType)
{
	UQuickTweenBuilderObject::SetLoopTypeBase(loopType);
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetEaseType(EEaseType easeType)
{
	UQuickTweenBuilderObject::SetEaseTypeBase(easeType);
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetEaseCurve(UCurveFloat* easeCurve)
{
	UQuickTweenBuilderObject::SetEaseCurveBase(easeCurve);
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetTimeScale(float timeScale)
{
	UQuickTweenBuilderObject::SetTimeScaleBase(timeScale);
	return this;
}

UQuickTweenBuilderMaterial* UQuickTweenBuilderMaterial::SetIsBackwards(bool bIsBackwards)
{
	UQuickTweenBuilderObject::SetIsBackwardsBase(bIsBackwards);
	return this;
}
