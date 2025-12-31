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

void UQuickTweenSequence::Evaluate(const FQuickTweenEvaluatePayload& payload, const UQuickTweenable* instigator)
{
	if (!HasOwner() || !InstigatorIsOwner(instigator)) return;

	bIsReversed = payload.bIsReversed;
	ElapsedTime = FMath::Clamp(payload.Value * GetTotalDuration(), 0.f, GetTotalDuration());

	if (bWasActive != payload.bIsActive)
	{
		auto simulateOnStart = [&]()
		{
			CurrentLoop = bIsReversed ? GetLoops() - 1 : 0;
			CurrentAlpha = bIsReversed ? 1.0f : 0.0f;
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

		if (payload.bIsActive)
		{
			simulateOnStart();
		}
		else
		{
			shouldSimulateOnComplete();
		}
		bWasActive = payload.bIsActive;
	}

	if (!payload.bIsActive)
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

	if (result.Loop != 0 && FMath::IsNearlyZero(localTime))
	{
		localTime  = GetLoopDuration();
	}

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
	const float fromTime = CurrentAlpha * GetLoopDuration();
	const float toTime = alpha * GetLoopDuration();
	const float deltaTime = FMath::Abs(toTime - fromTime);
	const bool bIsForward = toTime >= fromTime;

	const auto evaluateAtTime = [&](float sequenceTime)
	{
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
					FQuickTweenEvaluatePayload payload{
						.bIsActive = true,
						.bIsReversed = !bIsForward,
						.Value = childTime
					};
					tween->Evaluate(payload, this);
				}
				else
				{
					FQuickTweenEvaluatePayload payload{
						.bIsActive = false,
						.bIsReversed = !bIsForward,
						.Value = bIsForward ? 1.0f : 0.0f
					};
					tween->Evaluate(payload, this);
				}
			}
		}
	};

	constexpr float smallStepThreshold = 0.01f;

	// ... if the step is small enough, just evaluate directly
	if (deltaTime <= smallStepThreshold)
	{
		evaluateAtTime(toTime);
		CurrentAlpha = alpha;
		return;
	}

	// ... otherwise, collect breakpoints and evaluate at each
	TArray<float> breakpoints;
	breakpoints.Add(toTime);

	// ... collect breakpoints
	[&](TArray<float>& points)
	{
		const float Min = FMath::Min(fromTime, toTime);
		const float Max = FMath::Max(fromTime, toTime);

		for (const FQuickTweenSequenceGroup& group : TweenGroups)
		{
			for (UQuickTweenable* tween : group.Tweens)
			{
				if (!tween) continue;

				const float start = group.StartTime;
				const float end   = start + tween->GetTotalDuration();

				if (start > Min && start < Max)
					points.AddUnique(start);

				if (end > Min && end < Max)
					points.AddUnique(end);
			}
		}
	}(breakpoints);

	breakpoints.Sort();

	if (!bIsForward)
	{
		Algo::Reverse(breakpoints);
	}

	for (float point : breakpoints)
	{
		evaluateAtTime(point);
	}

	CurrentAlpha = alpha;
}

float UQuickTweenSequence::GetLoopDuration() const
{
	float sum = 0.0f;
	for (const FQuickTweenSequenceGroup& group : TweenGroups)
	{
		sum += group.Duration;
	}
	return sum;
}

int32 UQuickTweenSequence::GetNumTweens() const
{
	int32 sum = 0;
	for (const FQuickTweenSequenceGroup& group : TweenGroups)
	{
		sum += group.Tweens.Num();
	}
	return sum;
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
			CurrentAlpha = bIsReversed ? 1.0f : 0.0f;
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
	if (HasOwner() || GetLoops() == INFINITE_LOOPS) return;

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

	bool bSnapToEnd = bSnapToEndOnComplete;
	if (GetIsReversed())
	{
		bSnapToEnd = !bSnapToEnd;
	}

	const bool bSnapToBeginning  = !bSnapToEnd || (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0);
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