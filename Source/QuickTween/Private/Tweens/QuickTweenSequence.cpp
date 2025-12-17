// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickTweenSequence.h"

#include "QuickTweenManager.h"
#include "Utils/CommonValues.h"

UQuickTweenSequence::~UQuickTweenSequence()
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
	ApplySequenceState(state, true);
}

void UQuickTweenSequence::Evaluate(bool bIsActive, float value, const UQuickTweenable* instigator)
{
	if (!HasOwner() || !InstigatorIsOwner(instigator)) return;

	if (bWasActive != bIsActive)
	{
		auto simulateOnStart = [&]()
		{
			CurrentLoop = instigator->GetIsReversed() ? GetLoops() - 1 : 0;
			HandleOnStart();
		};
		if (bIsActive)
		{
			simulateOnStart();
		}
		bWasActive = bIsActive;
	}

	if (!bIsActive)
	{
		return;
	}

	bIsReversed = instigator->GetIsReversed();
	ElapsedTime = value * GetTotalDuration();

	FQuickTweenSequenceStateResult state = ComputeSequenceState(ElapsedTime);
	ApplySequenceState(state, false);
}

UQuickTweenSequence::FQuickTweenSequenceStateResult UQuickTweenSequence::ComputeSequenceState(float time) const
{
	FQuickTweenSequenceStateResult result;

	result.ElapsedTime = time;

	const float loopDuration = GetLoopDuration();

	result.Loop = FMath::FloorToInt(result.ElapsedTime / loopDuration);

	float localTime = FMath::Fmod(result.ElapsedTime, loopDuration);
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

void UQuickTweenSequence::ApplySequenceState(const FQuickTweenSequenceStateResult& state, bool bShouldUpdateTweenState)
{
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

void UQuickTweenSequence::ApplyAlphaValue(float alpha)
{
	float sequenceTime = alpha * GetLoopDuration();
	for (FQuickTweenSequenceGroup& group : TweenGroups)
	{
		const bool bIsGroupActive = sequenceTime >= group.StartTime && sequenceTime < (group.StartTime + group.Duration);
		for (UQuickTweenable* tween : group.Tweens)
		{
			const float childTime = (sequenceTime - group.StartTime) / tween->GetTotalDuration();
			tween->Evaluate(bIsGroupActive, childTime, this);
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