// Fill out your copyright notice in the Description page of Project Settings.
#include "Tweens/QuickTweenBase.h"

#include "Utils/CommonValues.h"

void UQuickTweenBase::Initialize(
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag)
{
	Duration = duration;
	TimeScale = timeScale;
	EaseType = easeType;
	EaseCurve = easeCurve;
	Loops = loops;
	LoopType = loopType;
	TweenTag = tweenTag;
}

void UQuickTweenBase::Update(float deltaTime)
{
	if (GetIsCompleted() || !GetIsPlaying()) return;

	ElapsedTime += deltaTime * GetTimeScale();

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this, Progress);
	}

	if (ElapsedTime >= (GetDuration() * GetCurrentLoop()))
	{
		if (Loops != INFINITE_LOOPS && CurrentLoop >= Loops)
		{
			Complete();
			return;
		}

		switch (LoopType)
		{
		case ELoopType::Restart:
			// Nothing to do here, just continue to the next loop
			break;
		case ELoopType::PingPong:
			Reverse();
			break;
		default:
			ensureAlwaysMsgf(false, TEXT("LoopType %s is not implemented in UQuickTweenBase::Update"), *UEnum::GetValueAsString(LoopType));
		}
		CurrentLoop++;
	}
	return;
}

UQuickTweenBase* UQuickTweenBase::Play()
{
	// TODO: Try to identify who is calling this in case the tween is in a sequence
	bIsPlaying = true;
	return this;
}

UQuickTweenBase* UQuickTweenBase::Pause()
{
	bIsPlaying = false;
	return this;
}

UQuickTweenBase* UQuickTweenBase::Stop()
{
	bIsPlaying = false;
	bIsCompleted = true;
	Restart();
	CurrentLoop = 1;
	return this;
}

UQuickTweenBase* UQuickTweenBase::Reverse()
{
	bIsBackwards = !bIsBackwards;
	return this;
}

UQuickTweenBase* UQuickTweenBase::Restart()
{
	ElapsedTime = bIsBackwards ? Duration : 0.0f;
	Progress = bIsBackwards ? 1.0f : 0.0f;
	return this;
}

UQuickTweenBase* UQuickTweenBase::Complete()
{
	Stop();
	if (OnComplete.IsBound())
	{
		OnComplete.Broadcast(this);
	}
	return this;
}

UQuickTweenBase* UQuickTweenBase::Reset()
{
	ElapsedTime = 0.0f;
	Progress = 0.0f;
	Duration = 0.0f;
	TimeScale = 1.0f;
	bIsPlaying = false;
	bIsCompleted = false;
	bIsBackwards = false;
	EaseType = EEaseType::Linear;
	EaseCurve = nullptr;
	Loops = 0;
	LoopType = ELoopType::Restart;
	CurrentLoop = 1;
	return this;
}

void UQuickTweenBase::Kill()
{
	bIsPendingKill = true;
}
