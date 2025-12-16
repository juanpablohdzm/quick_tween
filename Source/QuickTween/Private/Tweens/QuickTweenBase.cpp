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
		RequestStateTransition(EQuickTweenState::Play);
	}
}

void UQuickTweenBase::Update(float deltaTime)
{
	if (HasOwner()) return;

	ElapsedTime += (bIsReversed ? -1.f : 1.f) * deltaTime * GetTimeScale();

	FQuickTweenStateResult state = ComputeTweenState(ElapsedTime);
	ApplyTweenState(state, true);
}

void UQuickTweenBase::Evaluate(float value, const UQuickTweenable* instigator)
{
	if (!HasOwner() || !InstigatorIsOwner(instigator)) return;

	ElapsedTime = value * GetTotalDuration();

	FQuickTweenStateResult state = ComputeTweenState(ElapsedTime);
	ApplyTweenState(state, false);
}

UQuickTweenBase::FQuickTweenStateResult UQuickTweenBase::ComputeTweenState(float time) const
{
	FQuickTweenStateResult result;

	result.ElapsedTime = time;
	result.Loop  = FMath::FloorToInt(result.ElapsedTime /GetLoopDuration());


	float localTime = FMath::Fmod(result.ElapsedTime, GetLoopDuration());
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

void UQuickTweenBase::ApplyTweenState(const FQuickTweenStateResult& state, bool bShouldUpdateTweenState)
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
		auto HandleCompletion = [&](bool bUpdateState)
		{
			if (bUpdateState)
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

			HandleOnComplete();
		};

		if (!bIsReversed && CurrentLoop >= Loops)
		{
			HandleCompletion(bShouldUpdateTweenState);
			return;
		}

		if (bIsReversed && ElapsedTime < 0.0f)
		{
			HandleCompletion(bShouldUpdateTweenState);
			return;
		}
	}

	ApplyAlphaValue(state.Alpha);

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}
}

void UQuickTweenBase::ApplyAlphaValue(float alpha)
{
	ensureAlwaysMsgf(false, TEXT("UQuickTweenBase::ApplyAlphaValue: This method should be overridden in derived classes."));
}

void UQuickTweenBase::Play()
{
	if (HasOwner()) return;

	RequestStateTransition(EQuickTweenState::Play);
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

void UQuickTweenBase::Complete(bool bSnapToEnd)
{
	if (HasOwner()) return;

	bSnapToEndOnComplete = bSnapToEnd;
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
	ElapsedTime = bIsReversed ? GetTotalDuration() : 0.0f;
	CurrentLoop = bIsReversed ? GetLoops() - 1 : 0;
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