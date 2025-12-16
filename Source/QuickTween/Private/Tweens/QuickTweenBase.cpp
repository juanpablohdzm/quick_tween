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

	if (!ensureAlwaysMsgf(Duration > 0.f, TEXT("UQuickTweenBase::SetUp: Duration must be greater than zero, default to one.")))
	{
		Duration = 1.f;
	}

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
	if (!InstigatorIsOwner(instigator) || !GetIsPlaying()) return;

	ElapsedTime += (bIsReversed ? -1.f : 1.f) * deltaTime * GetTimeScale();

	FQuickTweenStateResult state = TickTween(this, ElapsedTime);
	ApplyTweenState(state, instigator);
}

void UQuickTweenBase::Evaluate(float value,  UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	const float previousElapsedTime = ElapsedTime;
	ElapsedTime = value * GetTotalDuration();

	bIsReversed = ElapsedTime < previousElapsedTime;

	if (!bIsReversed)
	{
		if (previousElapsedTime < 0.0f && ElapsedTime >= 0.0f)
		{
			Restart(instigator);
			Play(instigator);
		}
	}
	else
	{
		if (previousElapsedTime > GetTotalDuration() && ElapsedTime <= GetTotalDuration())
		{
			Restart(instigator);
			Play(instigator);
		}
	}

	if (!GetIsPlaying())
	{
		return;
	}

	FQuickTweenStateResult state = TickTween(this, ElapsedTime);
	ApplyTweenState(state, instigator);

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

void UQuickTweenBase::HandleOnIdleTransition()
{

}

void UQuickTweenBase::HandleOnStartTransition()
{
	ElapsedTime = bIsReversed ? GetTotalDuration() : 0.0f;
	CurrentLoop = bIsReversed ? GetLoops() - 1 : 0;
	if (OnStart.IsBound())
	{
		OnStart.Broadcast(this);
	}
	RequestStateTransition(EQuickTweenState::Play);
}

void UQuickTweenBase::HandleOnPlayTransition()
{
}

void UQuickTweenBase::HandleOnPauseTransition()
{
}

void UQuickTweenBase::HandleOnCompleteTransition(bool bSnapToEnd)
{
	ElapsedTime = bIsReversed ? 0.0f : GetTotalDuration();

	if (OnComplete.IsBound())
	{
		OnComplete.Broadcast(this);
	}

	if (bAutoKill)
	{
		Kill();
	}
}

void UQuickTweenBase::HandleOnKillTransition()
{
	if (OnKilled.IsBound())
	{
		OnKilled.Broadcast(this);
	}
}

UQuickTweenBase::FQuickTweenStateResult UQuickTweenBase::TickTween(UQuickTweenable* tween, float time)
{
	FQuickTweenStateResult result;

	result.ElapsedTime = time;
	result.Loop  = FMath::FloorToInt(result.ElapsedTime / tween->GetLoopDuration());


	float localTime = FMath::Fmod(result.ElapsedTime, tween->GetLoopDuration());
	if (localTime < 0.f)
	{
		localTime += tween->GetLoopDuration();
	}

	result.Alpha = localTime / tween->GetLoopDuration();
	if (tween->GetLoopType() == ELoopType::PingPong && (result.Loop & 1) != 0) // ... odd loop, backward traversal
	{
		result.Alpha = 1.f - result.Alpha;
	}

	return result;
}

void UQuickTweenBase::ApplyTweenState(const FQuickTweenStateResult& state, UQuickTweenable* instigator)
{
	const int32 numLoopsCrossed = FMath::Abs(state.Loop - CurrentLoop);
	if (OnLoop.IsBound())
	{
		for (int32 i = 0; i < numLoopsCrossed; ++i)
		{
			OnLoop.Broadcast(this);
		}
	}
	CurrentLoop = state.Loop;

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

	ApplyAlphaValue(state.Alpha);

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}
}

void UQuickTweenBase::Play(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (TweenState == EQuickTweenState::Idle)
	{
		RequestStateTransition(EQuickTweenState::Start);
	}
	else
	{
		RequestStateTransition(EQuickTweenState::Play);
	}
}

void UQuickTweenBase::Pause(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	RequestStateTransition(EQuickTweenState::Pause);
}

void UQuickTweenBase::Reverse(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bIsReversed = !bIsReversed;
}

void UQuickTweenBase::Restart(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	RequestStateTransition(EQuickTweenState::Idle);
}

void UQuickTweenBase::Complete(UQuickTweenable* instigator, bool bSnapToEnd)
{
	if (!InstigatorIsOwner(instigator)) return;

	RequestStateTransition(EQuickTweenState::Complete, bSnapToEnd);
}

void UQuickTweenBase::Kill(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	RequestStateTransition(EQuickTweenState::Kill);
}
