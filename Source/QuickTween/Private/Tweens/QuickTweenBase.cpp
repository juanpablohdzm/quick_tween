// Fill out your copyright notice in the Description page of Project Settings.
#include "Tweens/QuickTweenBase.h"

#include "QuickTweenManager.h"
#include "Utils/CommonValues.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenBase, Log, All);

void UQuickTweenBase::SetUp(
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	const UObject* worldContextObject,
	bool bShouldAutoKill)
{
	Duration = duration;
	TimeScale = timeScale;
	EaseType = easeType;
	EaseCurve = easeCurve;
	Loops = loops;
	LoopType = loopType;
	TweenTag = tweenTag;
	bAutoKill = bShouldAutoKill;

	UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject);
	if (!manager)
	{
		UE_LOG(LogQuickTweenBase, Log, TEXT("Failed to get QuickTweenManager for QuickTweenBase. Tweens will not be updated."));
	}
	else
	{
		manager->AddTween(this, Badge<UQuickTweenBase>());
	}
}

void UQuickTweenBase::Update(float deltaTime)
{
	Update(deltaTime, nullptr);
}

void UQuickTweenBase::Update_Restart(float deltaTime, Badge<UQuickTweenSequence>* badge)
{
	ElapsedTime = !bIsReversed ?  ElapsedTime + deltaTime * GetTimeScale() : ElapsedTime - deltaTime * GetTimeScale();

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this, Progress);
	}


	bool shouldComplete = false;
	if (!bIsReversed)
	{
		CurrentLoop = (ElapsedTime / GetDuration()) + 1;
		shouldComplete = CurrentLoop > Loops;
	}
	else
	{
		if (ElapsedTime < 0.0f)
		{
			shouldComplete = true;
		}
		else
		{
			const float mod = FMath::Fmod(ElapsedTime, GetDuration());
			const int32 loop = ElapsedTime / GetDuration();
			CurrentLoop = loop + 1;
			shouldComplete = loop <= 0 && FMath::IsNearlyZero(mod);
		}
	}

	if (Loops != INFINITE_LOOPS && shouldComplete)
	{
		Complete(badge);
	}
}

void UQuickTweenBase::Update_PingPong(float deltaTime, Badge<UQuickTweenSequence>* badge)
{
	/*
	 * !backwards & !reversed = to end
	 *  backwards & reversed = to end
	 * !backwards & reversed = to beginning
	 * !backwards & reversed = to beginning
	 *
	 * */
	const bool isGoingForward = bIsBackwards == bIsReversed;

	bool shouldCompleteLoop = false;
	if (isGoingForward)
	{
		ElapsedTime += deltaTime * GetTimeScale();
		if (ElapsedTime >= GetDuration())
		{
			shouldCompleteLoop = true;
			ElapsedTime = GetDuration() - (ElapsedTime - GetDuration());
		}
	}
	else
	{
		ElapsedTime -= deltaTime * GetTimeScale();
		if (ElapsedTime <= 0.0f)
		{
			shouldCompleteLoop = true;
			ElapsedTime = -ElapsedTime;
		}
	}

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this, Progress);
	}

	if (shouldCompleteLoop)
	{
		if (Loops != INFINITE_LOOPS && (!bIsReversed && CurrentLoop >= Loops || bIsReversed && (CurrentLoop - 1) <= 0))
		{
			Complete(badge);
			return;
		}

		bIsBackwards = !bIsBackwards;
		CurrentLoop = !bIsReversed ? CurrentLoop + 1 : CurrentLoop - 1;
	}
}

void UQuickTweenBase::Update(float deltaTime, Badge<UQuickTweenSequence>* badge)
{
	if (GetIsCompleted() || !GetIsPlaying()) return;

	if (FMath::IsNearlyZero(GetDuration()))
	{
		Complete(badge);
		return;
	}

	switch (LoopType)
	{
		case ELoopType::Restart:
			Update_Restart(deltaTime, badge);
			break;
		case ELoopType::PingPong:
			Update_PingPong(deltaTime, badge);
			break;
		default:
			ensureAlwaysMsgf(false, TEXT("LoopType %s is not implemented in UQuickTweenBase::Update"), *UEnum::GetValueAsString(LoopType));
	}
}

void UQuickTweenBase::SetAutoKill(bool bShouldAutoKill)
{
	bAutoKill = bShouldAutoKill;
	if (GetIsCompleted() && !bIsInSequence)
	{
		bIsPendingKill = bAutoKill;
	}
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

	bIsReversed = !bIsReversed;

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

	float duration = GetLoopType() == ELoopType::PingPong ? GetDuration() : GetDuration() * GetLoops();
	ElapsedTime = bIsReversed ? duration : 0.0f;
	Progress    = bIsReversed ? 1.0f     : 0.0f;
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

	const bool toEnd = bIsReversed == bIsBackwards;

	bIsPlaying   = false;
	bIsCompleted = true;

	ElapsedTime = toEnd ? Duration : 0.0f;
	Progress    = toEnd ? 1.0f     : 0.0f;
	if (bAutoKill)
	{
		Kill(badge);
	}

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
	bIsReversed = false;
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
	if (bIsInSequence) return;
	if (bIsPendingKill) return;

	bIsPendingKill = true;
	bIsPlaying     = false;
}
