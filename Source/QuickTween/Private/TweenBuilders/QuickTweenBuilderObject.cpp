// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenBuilders/QuickTweenBuilderObject.h"

#include "Tweens/QuickTweenBase.h"
#include "Tweens/QuickTweenSequence.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenBuilder, Log, All);

void UQuickTweenBuilderObject::Initialize(UObject* inObject)
{
	Target = inObject;
	Sequence = NewObject<UQuickTweenSequence>();
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::SetUp(
	const UObject* worldContextObject,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag)
{
	Sequence->SetUp(worldContextObject, loops, loopType, tweenTag);
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::AppendBase()
{
	bIsAppend = true;
	return this;
}

UQuickTweenBuilderObject* UQuickTweenBuilderObject::JoinBase()
{
	bIsAppend = false;
	return this;
}
