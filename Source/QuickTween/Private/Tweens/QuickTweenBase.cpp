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
	Update(deltaTime, nullptr);
}

void UQuickTweenBase::Update(float deltaTime, Badge<UQuickTweenSequence>* badge)
{
	if (GetIsCompleted() || !GetIsPlaying()) return;

	ElapsedTime = !bIsBackwards?  ElapsedTime + deltaTime * GetTimeScale() : ElapsedTime - deltaTime * GetTimeScale();

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this, Progress);
	}

	bool shouldCompleteLoop;
	if (LoopType == ELoopType::PingPong)
	{
		shouldCompleteLoop = !bIsBackwards ? ElapsedTime >= GetDuration() : ElapsedTime <= 0.0f;
	}
	else
	{
		shouldCompleteLoop = !bIsBackwards ? ElapsedTime >= GetDuration() * GetCurrentLoop() : ElapsedTime <= -(GetDuration() * (GetCurrentLoop() - 1));
	}

	if (shouldCompleteLoop)
	{
		if (Loops != INFINITE_LOOPS && CurrentLoop >= Loops)
		{
			Complete(badge);
			return;
		}

		switch (LoopType)
		{
			case ELoopType::Restart:
				// Nothing to do here, just continue to the next loop
				break;
			case ELoopType::PingPong:
				bIsBackwards = !bIsBackwards;
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
	return Play(nullptr);
}

UQuickTweenBase* UQuickTweenBase::Play(Badge<UQuickTweenSequence>* badge)
{
	if (bIsInSequence && !badge) return this;

	bIsPlaying = true;
	return this;
}

UQuickTweenBase* UQuickTweenBase::Pause()
{
	return Pause(nullptr);
}

UQuickTweenBase* UQuickTweenBase::Pause(Badge<UQuickTweenSequence>* badge)
{
	if (bIsInSequence && !badge) return this;

	bIsPlaying = false;
	return this;
}

UQuickTweenBase* UQuickTweenBase::Stop()
{
	return Stop(nullptr);
}

UQuickTweenBase* UQuickTweenBase::Stop(Badge<UQuickTweenSequence>* badge)
{

	if (bIsInSequence && !badge) return this;

	if (!bIsPlaying && !bIsCompleted)
	{
		return this;
	}

	bIsPlaying   = false;
	bIsCompleted = true;
	CurrentLoop  = 1;
	Restart(badge);

	return this;
}

UQuickTweenBase* UQuickTweenBase::Reverse()
{
	return Reverse(nullptr);
}

UQuickTweenBase* UQuickTweenBase::Reverse(Badge<UQuickTweenSequence>* badge)
{
	if (bIsInSequence && !badge) return this;

	bIsBackwards = !bIsBackwards;

	return this;
}

UQuickTweenBase* UQuickTweenBase::Restart()
{
	return Restart(nullptr);
}

UQuickTweenBase* UQuickTweenBase::Restart(Badge<UQuickTweenSequence>* badge)
{
	if (bIsInSequence && !badge) return this;

	bIsCompleted = false;
	bIsPlaying   = true;

	ElapsedTime = bIsBackwards ? Duration : 0.0f;
	Progress    = bIsBackwards ? 1.0f     : 0.0f;
	return this;
}

UQuickTweenBase* UQuickTweenBase::Complete()
{
	return Complete(nullptr);
}

UQuickTweenBase* UQuickTweenBase::Complete(Badge<UQuickTweenSequence>* badge)
{
	if (bIsInSequence && !badge) return this;
	if (bIsCompleted) return this;

	const bool endsAtEndByLoop = (LoopType != ELoopType::PingPong) || ((Loops % 2) == 1);
	const bool toEnd = bIsBackwards ? false : endsAtEndByLoop;

	bIsPlaying   = false;
	bIsCompleted = true;

	ElapsedTime = toEnd ? Duration : 0.0f;
	Progress    = toEnd ? 1.0f     : 0.0f;

	if (OnComplete.IsBound())
	{
		OnComplete.Broadcast(this);
	}
	return this;
}

UQuickTweenBase* UQuickTweenBase::Reset()
{
	return Reset(nullptr);
}

UQuickTweenBase* UQuickTweenBase::Reset(Badge<UQuickTweenSequence>* badge)
{
	if (bIsInSequence && !badge) return this;

	ElapsedTime = 0.0f;
	Progress    = 0.0f;
	Duration    = 0.0f;
	TimeScale   = 1.0f;
	bIsPlaying  = false;
	bIsCompleted= false;
	bIsBackwards= false;
	EaseType    = EEaseType::Linear;
	EaseCurve   = nullptr;
	Loops       = 0;
	LoopType    = ELoopType::Restart;
	CurrentLoop = 1;
	return this;
}

void UQuickTweenBase::Kill()
{
	Kill(nullptr);
}

void UQuickTweenBase::Kill(Badge<UQuickTweenSequence>* badge)
{
	if (bIsInSequence && !badge) return;
	if (bIsPendingKill) return;

	bIsPendingKill = true;
	bIsPlaying     = false;
}
