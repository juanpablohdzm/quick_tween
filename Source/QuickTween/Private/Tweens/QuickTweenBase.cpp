// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#include "Tweens/QuickTweenBase.h"

#include "QuickTweenManager.h"
#include "Utils/CommonValues.h"

UQuickTweenBase::~UQuickTweenBase()
{
	if (!WorldContextObject || Owner)
	{
		return;
	}

	if (UQuickTweenManager* manager = UQuickTweenManager::Get(WorldContextObject))
	{
		manager->RemoveTween(this);
	}
}

void UQuickTweenBase::SetUp(
	const UObject* worldContextObject,
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	WorldContextObject = worldContextObject;
	Duration = duration;
	TimeScale = timeScale;
	EaseType = easeType;
	EaseCurve = easeCurve;
	Loops = loops;
	LoopType = loopType;
	TweenTag = tweenTag;
	bAutoKill = bShouldAutoKill;
	bPlayWhilePaused = bShouldPlayWhilePaused;

	UQuickTweenManager* manager = UQuickTweenManager::Get(WorldContextObject);
	if (!manager)
	{
		UE_LOG(LogQuickTweenBase, Log, TEXT("Failed to get QuickTweenManager for QuickTweenBase. Tweens will not be updated."));
	}
	else
	{
		manager->AddTween(this);
	}

	if (bShouldAutoPlay)
	{
		Play();
	}
}

void UQuickTweenBase::Update(float deltaTime, UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator) || GetIsCompleted() || !GetIsPlaying()) return;

	if (!bHasStarted)
	{
		bHasStarted = true;
		if (OnStart.IsBound())
		{
			OnStart.Broadcast(this);
		}
	}

	if (FMath::IsNearlyZero(GetDuration()))
	{
		Complete(instigator);
		return;
	}

	ElapsedTime += (bIsReversed ? -1.f : 1.f) * deltaTime * GetTimeScale();

	const int32 loop  = FMath::FloorToInt(ElapsedTime / Duration);

	// ... in case deltaTime is large enough to cross multiple loops in a single update
	const int32 numLoopsCrossed = FMath::Abs(loop - CurrentLoop);
	if (numLoopsCrossed > 0)
	{
		CurrentLoop = loop;

		if (OnLoop.IsBound())
		{
			for (int32 i = 0; i < numLoopsCrossed; ++i)
			{
				OnLoop.Broadcast(this);
			}
		}
	}

	// ... check for completion
	if (Loops != INFINITE_LOOPS)
	{
		if (!bIsReversed)
		{
			if (CurrentLoop >= Loops)
			{
				Complete(instigator);
				return;
			}
		}
		else
		{
			if (ElapsedTime < 0.0f)
			{
				Complete(instigator);
				return;
			}
		}
	}

	// ... value [0 .. Duration)
	float localTime = FMath::Fmod(ElapsedTime, Duration);
	if (localTime < 0.f)
	{
		localTime += Duration;
	}

	float alphaValue = localTime / Duration;
	if (LoopType == ELoopType::PingPong && (CurrentLoop & 1) != 0) // ... odd loop, backward traversal
	{
		alphaValue = 1.f - alphaValue;
	}

	ApplyAlphaValue(alphaValue);

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}
}

void UQuickTweenBase::AssignOnStartEvent(FDynamicDelegateTween callback)
{
	OnStart.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenBase::AssignOnUpdateEvent(FDynamicDelegateTween callback)
{
	OnUpdate.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenBase::AssignOnCompleteEvent(FDynamicDelegateTween callback)
{
	OnComplete.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenBase::AssignOnKilledEvent(FDynamicDelegateTween callback)
{
	OnKilled.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenBase::AssignOnLoopEvent(FDynamicDelegateTween callback)
{
	OnLoop.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenBase::RemoveAllOnStartEvent(const UObject* object)
{
	OnStart.RemoveAll(object);
}

void UQuickTweenBase::RemoveAllOnUpdateEvent(const UObject* object)
{
	OnUpdate.RemoveAll(object);
}

void UQuickTweenBase::RemoveAllOnCompleteEvent(const UObject* object)
{
	OnComplete.RemoveAll(object);
}

void UQuickTweenBase::RemoveAllOnKilledEvent(const UObject* object)
{
	OnKilled.RemoveAll(object);
}

void UQuickTweenBase::RemoveAllOnLoopEvent(const UObject* object)
{
	OnLoop.RemoveAll(object);
}

void UQuickTweenBase::ApplyAlphaValue(float alpha)
{
	ensureAlwaysMsgf(false, TEXT("UQuickTweenBase::ApplyAlphaValue: This method should be overridden in derived classes."));
}

void UQuickTweenBase::RequestStateTransition(EQuickTweenState newState)
{
	if (newState == TweenState) return;

	static TMap<EQuickTweenState, TArray<EQuickTweenState>> validTransitions =
	{
		{EQuickTweenState::Idle, {EQuickTweenState::Play, EQuickTweenState::Kill}},
		{EQuickTweenState::Play,    {EQuickTweenState::Pause, EQuickTweenState::Complete, EQuickTweenState::Kill, EQuickTweenState::Idle}},
		{EQuickTweenState::Pause,     {EQuickTweenState::Play, EQuickTweenState::Complete, EQuickTweenState::Kill, EQuickTweenState::Idle}},
		{EQuickTweenState::Complete,  {EQuickTweenState::Idle}},
		{EQuickTweenState::Kill,     {}},
	};

	if (validTransitions[TweenState].Contains(newState))
	{
		switch (newState)
		{
		case EQuickTweenState::Idle:
			break;
		case EQuickTweenState::Start:
			break;
		case EQuickTweenState::Play:
			break;
		case EQuickTweenState::Pause:
			break;
		case EQuickTweenState::Complete:
			break;
		case EQuickTweenState::Kill:
			break;
		}
	}
	else
	{
		UE_LOG(LogQuickTweenBase, Warning, TEXT("Invalid state transition from %s to %s"),  *UEnum::GetValueAsString(TweenState), *UEnum::GetValueAsString(newState));
	}
}

void UQuickTweenBase::Play(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bIsPlaying = true;
	return;
}

void UQuickTweenBase::Pause(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bIsPlaying = false;
	return;
}

void UQuickTweenBase::Reverse(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bIsReversed = !bIsReversed;

	return;
}

void UQuickTweenBase::Restart(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bIsCompleted = false;
	bIsPlaying   = true;
	bHasStarted = false;

	float duration = GetLoopType() == ELoopType::PingPong ? GetDuration() : GetDuration() * GetLoops();
	ElapsedTime = bIsReversed ? duration : 0.0f;
	CurrentLoop = bIsReversed ? GetLoops() : 1;
	return;
}

void UQuickTweenBase::Complete(UQuickTweenable* instigator, bool bSnapToEnd)
{
	if (!InstigatorIsOwner(instigator)) return;
	if (bIsCompleted) return;

	bIsPlaying   = false;
	bIsCompleted = true;

	ElapsedTime = Duration;

	if (OnComplete.IsBound())
	{
		OnComplete.Broadcast(this);
	}

	if (bAutoKill)
	{
		Kill(nullptr);
	}

	return;
}

void UQuickTweenBase::Kill(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;
	if (bIsPendingKill) return;

	bIsPendingKill = true;
	bIsPlaying     = false;

	if (OnKilled.IsBound())
	{
		OnKilled.Broadcast(this);
	}
}
