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
	if (!InstigatorIsOwner(instigator) || !GetIsPlaying()) return;

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

void UQuickTweenBase::HandleOnIdleTransition()
{

}

void UQuickTweenBase::HandleOnStartTransition()
{
	ElapsedTime = bIsReversed ? GetDuration() * GetLoops() : 0.0f;
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
	ElapsedTime = bIsReversed ? 0.0f : GetDuration() * GetLoops();

	if (OnComplete.IsBound())
	{
		OnComplete.Broadcast(this);
	}

	if (bAutoKill)
	{
		RequestStateTransition(EQuickTweenState::Kill);
	}
}

void UQuickTweenBase::HandleOnKillTransition()
{
	if (OnKilled.IsBound())
	{
		OnKilled.Broadcast(this);
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

	RequestStateTransition(EQuickTweenState::Complete);
}

void UQuickTweenBase::Kill(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	RequestStateTransition(EQuickTweenState::Kill);
}
