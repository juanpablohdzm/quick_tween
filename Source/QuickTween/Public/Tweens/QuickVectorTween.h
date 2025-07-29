// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenBase.h"
#include "QuickVectorTween.generated.h"

/**
 * 
 */
UCLASS()
class QUICKTWEEN_API UQuickVectorTween : public UQuickTweenBase
{
	GENERATED_BODY()
public:

	void Initialize(
		const FVector& from,
		const FVector& to,
		TFunction<void(const FVector&)> setterFunction,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = FString())
	{
		From = from;
		To = to;
		SetterFunction = setterFunction;
		UQuickTweenBase::Initialize(
			duration,
			timeScale,
			easeType,
			easeCurve,
			loops,
			loopType,
			tweenTag);

	}

	virtual bool Update(float deltaTime) override;
	virtual UQuickTweenBase* Complete() override;

private:
	FVector From;
	FVector To;
	TFunction<void(const FVector&)> SetterFunction;
};
