// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#include "Tweens/QuickTweenBase.h"

#include "QuickTweenManager.h"
#include "Utils/CommonValues.h"

UQuickTweenBase::~UQuickTweenBase()
{
	if (HasOwner() || !WorldContextObject)
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
	bool bShouldAutoPlay,
	bool bShouldSnapToEndOnComplete)
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
	bSnapToEndOnComplete = bShouldSnapToEndOnComplete;

	if (!ensureAlwaysMsgf(WorldContextObject, TEXT("UQuickTweenBase::SetUp: WorldContextObject is null.")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(Duration > 0.f, TEXT("UQuickTweenBase::SetUp: Duration must be greater than zero, default to one.")))
	{
		Duration = 1.f;
	}

	if (!ensureAlwaysMsgf(TimeScale > 0.f, TEXT("UQuickTweenBase::SetUp: TimeScale must be greater than zero, default to one.")))
	{
		TimeScale = 1.f;
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
		RequestStateTransition(EQuickTweenState::Play);
	}
}

void UQuickTweenBase::Update(float deltaTime)
{
	if (HasOwner()) return;

	ElapsedTime += (bIsReversed ? -1.f : 1.f) * deltaTime * GetTimeScale(); // ... we allow overflow to handle completion properly

	FQuickTweenStateResult state = ComputeTweenState(ElapsedTime);

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
		if ((!bIsReversed && CurrentLoop >= Loops) || (bIsReversed && ElapsedTime < 0.0f))
		{
			if (RequestStateTransition(EQuickTweenState::Complete))
			{
				HandleOnComplete();
				if (bAutoKill && RequestStateTransition(EQuickTweenState::Kill))
				{
					HandleOnKill();
				}
			}
			return;
		}
	}

	ApplyAlphaValue(state.Alpha);

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}
}

void UQuickTweenBase::Evaluate(bool bIsActive, float value, const UQuickTweenable* instigator)
{
	if (!HasOwner() || !InstigatorIsOwner(instigator)) return;

	bIsReversed = instigator->GetIsReversed();
	ElapsedTime = FMath::Clamp(value * GetTotalDuration(), 0.f, GetTotalDuration());

	if (bWasActive != bIsActive)
	{
		auto simulateOnStart = [&]()
		{
			CurrentLoop = bIsReversed ? GetLoops() - 1 : 0;
			HandleOnStart();
		};

		auto shouldSimulateOnComplete = [&]()
		{
			if ((bIsReversed && FMath::IsNearlyZero(ElapsedTime)) ||
			    (!bIsReversed && FMath::IsNearlyEqual(ElapsedTime, GetTotalDuration())))
			{
				CurrentLoop = bIsReversed ? 0 : GetLoops() - 1;
				HandleOnComplete();
			}
		};

		if (bIsActive)
		{
			simulateOnStart();
		}
		else
		{
			shouldSimulateOnComplete();
		}
		bWasActive = bIsActive;
	}

	if (!bIsActive)
	{
		return;
	}

	FQuickTweenStateResult state = ComputeTweenState(ElapsedTime);

	const int32 numLoopsCrossed = FMath::Abs(state.Loop - CurrentLoop);
	if (OnLoop.IsBound())
	{
		for (int32 i = 0; i < numLoopsCrossed; ++i)
		{
			OnLoop.Broadcast(this);
		}
	}
	CurrentLoop = state.Loop;

	ApplyAlphaValue(state.Alpha);

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}
}

UQuickTweenBase::FQuickTweenStateResult UQuickTweenBase::ComputeTweenState(float time) const
{
	FQuickTweenStateResult result;

	result.Loop  = FMath::FloorToInt(time /GetLoopDuration());


	float localTime = FMath::Fmod(time, GetLoopDuration());

	if (result.Loop != 0 && FMath::IsNearlyZero(localTime))
	{
		localTime  = GetLoopDuration();
	}

	if (localTime < 0.f)
	{
		localTime += GetLoopDuration();
	}

	result.Alpha = localTime / GetLoopDuration();
	if (GetLoopType() == ELoopType::PingPong && (result.Loop & 1) != 0) // ... odd loop, backward traversal
	{
		result.Alpha = 1.f - result.Alpha;
	}

	return result;
}

void UQuickTweenBase::ApplyAlphaValue(float alpha)
{
	ensureAlwaysMsgf(false, TEXT("UQuickTweenBase::ApplyAlphaValue: This method should be overridden in derived classes."));
}

void UQuickTweenBase::Play()
{
	if (HasOwner()) return;

	const EQuickTweenState prevState = TweenState;
	if (RequestStateTransition(EQuickTweenState::Play))
	{
		if (prevState == EQuickTweenState::Idle)
		{
			ElapsedTime = bIsReversed ? GetTotalDuration() : 0.0f;
			CurrentLoop = bIsReversed ? GetLoops() - 1 : 0;
			HandleOnStart();
		}
	}
}

void UQuickTweenBase::Pause()
{
	if (HasOwner()) return;

	RequestStateTransition(EQuickTweenState::Pause);
}

void UQuickTweenBase::Reverse()
{
	if (HasOwner()) return;

	bIsReversed = !bIsReversed;
}

void UQuickTweenBase::Restart()
{
	if (HasOwner()) return;

	RequestStateTransition(EQuickTweenState::Idle);
}

void UQuickTweenBase::Complete()
{
	if (HasOwner() || GetLoops() == INFINITE_LOOPS ) return;

	if (RequestStateTransition(EQuickTweenState::Complete))
	{
		HandleOnComplete();
		if (bAutoKill)
		{
			if (RequestStateTransition(EQuickTweenState::Kill))
			{
				HandleOnKill();
			}
		}
	}
}

void UQuickTweenBase::Kill()
{
	if (HasOwner()) return;

	if (RequestStateTransition(EQuickTweenState::Kill))
	{
		HandleOnKill();
	}
}

bool UQuickTweenBase::RequestStateTransition(EQuickTweenState newState)
{
	if (newState == TweenState) return false;

	if (ValidTransitions[TweenState].Contains(newState))
	{
		TweenState = newState;
		return true;
	}

	UE_LOG(LogQuickTweenBase, Warning, TEXT("Invalid state transition from %s to %s"),  *UEnum::GetValueAsString(TweenState), *UEnum::GetValueAsString(newState));
	return false;
}

void UQuickTweenBase::HandleOnStart()
{
	if (OnStart.IsBound())
	{
		OnStart.Broadcast(this);
	}
}

void UQuickTweenBase::HandleOnComplete()
{
	ElapsedTime = bIsReversed ? 0.0f : GetTotalDuration();

	if (OnComplete.IsBound())
	{
		OnComplete.Broadcast(this);
	}
}

void UQuickTweenBase::HandleOnKill()
{
	if (OnKilled.IsBound())
	{
		OnKilled.Broadcast(this);
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