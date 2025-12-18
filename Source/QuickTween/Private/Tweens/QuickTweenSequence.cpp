// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickTweenSequence.h"

#include "QuickTweenManager.h"
#include "Utils/CommonValues.h"

UQuickTweenSequence::~UQuickTweenSequence()
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

void UQuickTweenSequence::SetUp(
	const UObject* worldContextObject,
	int32 loops,
	ELoopType loopType,
	const FString& id,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused)
{
	Loops = loops;
	LoopType = loopType;
	SequenceTweenId	= id;
	WorldContextObject = worldContextObject;
	bAutoKill	= bShouldAutoKill;
	bPlayWhilePaused = bShouldPlayWhilePaused;

	UQuickTweenManager* manager = UQuickTweenManager::Get(WorldContextObject);
	if (!manager)
	{
		UE_LOG(LogQuickTweenSequence, Log, TEXT("Failed to get QuickTweenManager for QuickTweenSequence. Tweens will not be updated."));
	}
	else
	{
		manager->AddTween(this);
	}
}

UQuickTweenSequence* UQuickTweenSequence::Join(UQuickTweenable* tween)
{
	if (!tween)
	{
		UE_LOG(LogQuickTweenSequence, Warning, TEXT("Join called with a null tween. This should never happen."));
		return this;
	}

	if (tween->GetLoops()  == INFINITE_LOOPS)
	{
		UE_LOG(LogQuickTweenSequence, Warning, TEXT("Joining a tween with infinite loops is not allowed. Please set a finite number of loops."));
		return this;
	}

	if (TweenGroups.Num() == 0)
	{
		return Append(tween);
	}

	if (UQuickTweenManager* manager = UQuickTweenManager::Get(WorldContextObject))
	{
		manager->RemoveTween(tween);
	}
	else
	{
		UE_LOG(LogQuickTweenSequence, Log, TEXT("Failed to get QuickTweenManager when joining a tween to sequence."));
	}
	tween->SetOwner(this);

	FQuickTweenSequenceGroup& lastGroup = TweenGroups.Last();
	lastGroup.Tweens.Add(tween);
	lastGroup.Duration = FMath::Max(lastGroup.Duration, tween->GetTotalDuration());
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Append(UQuickTweenable* tween)
{
	if (!tween)
	{
		UE_LOG(LogQuickTweenSequence, Warning, TEXT("Append called with a null tween. This should never happen."));
		return this;
	}

	if (tween->GetLoops()  == INFINITE_LOOPS)
	{
		UE_LOG(LogQuickTweenSequence, Warning, TEXT("Append a tween with infinite loops is not allowed. Please set a finite number of loops."));
		return this;
	}

	if (UQuickTweenManager* manager = UQuickTweenManager::Get(WorldContextObject))
	{
		manager->RemoveTween(tween);
	}
	else
	{
		UE_LOG(LogQuickTweenSequence, Log, TEXT("Failed to get QuickTweenManager when appending a tween to sequence."));
	}
	tween->SetOwner(this);

	FQuickTweenSequenceGroup group;
	group.Tweens.Add(tween);
	group.Duration = tween->GetTotalDuration();
	if (TweenGroups.Num() == 0)
	{
		group.StartTime = 0.f;
	}
	else
	{
		const FQuickTweenSequenceGroup& lastGroup = TweenGroups.Last();
		group.StartTime = lastGroup.StartTime + lastGroup.Duration;
	}

	TweenGroups.Add(group);
	return this;
}

void UQuickTweenSequence::Update(float deltaTime)
{
	if (HasOwner()) return;

	ElapsedTime += (bIsReversed ? -1.f : 1.f) * deltaTime;

	FQuickTweenSequenceStateResult state = ComputeSequenceState(ElapsedTime);

	// ... in case deltaTime is large enough to cross multiple loops in a single update
	if (state.Loop != CurrentLoop)
	{
		const int32 crossed = FMath::Abs(state.Loop - CurrentLoop);
		CurrentLoop = state.Loop;

		if (OnLoop.IsBound())
		{
			for (int32 i = 0; i < crossed; ++i)
			{
				OnLoop.Broadcast(this);
			}
		}
	}

	//... check for completion
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

void UQuickTweenSequence::Evaluate(bool bIsActive, float value, const UQuickTweenable* instigator)
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

	FQuickTweenSequenceStateResult state = ComputeSequenceState(ElapsedTime);

	// ... in case deltaTime is large enough to cross multiple loops in a single update
	if (state.Loop != CurrentLoop)
	{
		const int32 crossed = FMath::Abs(state.Loop - CurrentLoop);
		CurrentLoop = state.Loop;

		if (OnLoop.IsBound())
		{
			for (int32 i = 0; i < crossed; ++i)
			{
				OnLoop.Broadcast(this);
			}
		}
	}

	ApplyAlphaValue(state.Alpha);

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}
}

UQuickTweenSequence::FQuickTweenSequenceStateResult UQuickTweenSequence::ComputeSequenceState(float time) const
{
	FQuickTweenSequenceStateResult result;

	const float loopDuration = GetLoopDuration();

	result.Loop = FMath::FloorToInt(time / loopDuration);

	float localTime = FMath::Fmod(time, loopDuration);
	if (localTime < 0.f)
	{
		localTime += loopDuration;
	}

	result.Alpha = localTime / loopDuration;
	if (GetLoopType() == ELoopType::PingPong && (result.Loop & 1) != 0) // ... odd loop, backward traversal
	{
		result.Alpha = 1.f - result.Alpha;
	}

	return result;
}

void UQuickTweenSequence::ApplyAlphaValue(float alpha)
{
	float sequenceTime = alpha * GetLoopDuration();
	for (FQuickTweenSequenceGroup& group : TweenGroups)
	{
		const float startTime = group.StartTime;
		const float endTime = (group.StartTime + group.Duration);
		const bool bIsGroupActive = sequenceTime >= startTime && sequenceTime <= endTime;

		for (UQuickTweenable* tween : group.Tweens)
		{
			const bool bIsActive = bIsGroupActive &&  /*isSpecificTweenActive*/ (sequenceTime >= startTime && sequenceTime <= (startTime + tween->GetTotalDuration()));
			if (bIsActive)
			{
				const float childTime = (sequenceTime - group.StartTime) / tween->GetTotalDuration();
				tween->Evaluate(/*bIsActive*/ true, childTime, this);
			}
			else
			{
				tween->Evaluate(/*bIsActive*/ false, bIsReversed ? 0.0f : 1.0f, this);
			}
		}
	}
}

float UQuickTweenSequence::GetLoopDuration() const
{
	return Algo::Accumulate(TweenGroups, 0.0f, [](float sum, const FQuickTweenSequenceGroup& group)
	{
		return sum + group.Duration;
	});
}

int32 UQuickTweenSequence::GetNumTweens() const
{
	return Algo::Accumulate(TweenGroups, 0, [](int32 sum, const FQuickTweenSequenceGroup& group)
	{
		return sum + group.Tweens.Num();
	});
}

UQuickTweenable* UQuickTweenSequence::GetTween(int32 index) const
{
	int32 currentTweenIndex = 0;
	for (const FQuickTweenSequenceGroup& group : TweenGroups)
	{
		if (index < currentTweenIndex + group.Tweens.Num())
		{
			return group.Tweens[index - currentTweenIndex];
		}
		currentTweenIndex += group.Tweens.Num();
	}
	return nullptr;
}

void UQuickTweenSequence::Play()
{
	if (HasOwner()) return;

	const EQuickTweenState prevState = SequenceState;
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

void UQuickTweenSequence::Pause()
{
	if (HasOwner()) return;

	RequestStateTransition(EQuickTweenState::Pause);
}

void UQuickTweenSequence::Reverse()
{
	if (HasOwner()) return;

	bIsReversed = !bIsReversed;
}

void UQuickTweenSequence::Restart()
{
	if (HasOwner()) return;

	RequestStateTransition(EQuickTweenState::Idle);
}

void UQuickTweenSequence::Complete(bool bSnapToEnd)
{
	if (HasOwner()) return;

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

void UQuickTweenSequence::Kill()
{
	if (HasOwner()) return;

	if (RequestStateTransition(EQuickTweenState::Kill))
	{
		HandleOnKill();
	}
}

bool UQuickTweenSequence::RequestStateTransition(EQuickTweenState newState)
{
	if (newState == SequenceState) return false;

	if (ValidTransitions[SequenceState].Contains(newState))
	{
		SequenceState = newState;
		return true;
	}

	UE_LOG(LogQuickTweenSequence, Warning, TEXT("Invalid state transition from %s to %s"),  *UEnum::GetValueAsString(SequenceState), *UEnum::GetValueAsString(newState));
	return false;
}

void UQuickTweenSequence::HandleOnStart()
{
	if (OnStart.IsBound())
	{
		OnStart.Broadcast(this);
	}
}

void UQuickTweenSequence::HandleOnComplete()
{
	ElapsedTime = bIsReversed ? 0.0f : GetTotalDuration();

	const bool bSnapToBeginning  = bIsReversed || (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0);
	ApplyAlphaValue(bSnapToBeginning ? 0.0f : 1.f);

	if (OnComplete.IsBound())
	{
		OnComplete.Broadcast(this);
	}
}

void UQuickTweenSequence::HandleOnKill()
{
	if (OnKilled.IsBound())
	{
		OnKilled.Broadcast(this);
	}
}

void UQuickTweenSequence::AssignOnStartEvent(FDynamicDelegateTweenSequence callback)
{
	OnStart.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenSequence::AssignOnUpdateEvent(FDynamicDelegateTweenSequence callback)
{
	OnUpdate.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenSequence::AssignOnCompleteEvent(FDynamicDelegateTweenSequence callback)
{
	OnComplete.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenSequence::AssignOnKilledEvent(FDynamicDelegateTweenSequence callback)
{
	OnKilled.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenSequence::AssignOnLoopEvent(FDynamicDelegateTweenSequence callback)
{
	OnLoop.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenSequence::RemoveAllOnStartEvent(const UObject* object)
{
	OnStart.RemoveAll(object);
}

void UQuickTweenSequence::RemoveAllOnUpdateEvent(const UObject* object)
{
	OnUpdate.RemoveAll(object);
}

void UQuickTweenSequence::RemoveAllOnCompleteEvent(const UObject* object)
{
	OnComplete.RemoveAll(object);
}

void UQuickTweenSequence::RemoveAllOnKilledEvent(const UObject* object)
{
	OnKilled.RemoveAll(object);
}

void UQuickTweenSequence::RemoveAllOnLoopEvent(const UObject* object)
{
	OnLoop.RemoveAll(object);
}