// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Tweens/QuickTweenSequence.h"

#include "QuickTweenManager.h"
#include "Tweens/QuickTweenBase.h"
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
		UE_LOG(LogQuickTweenSequence, Log, TEXT("Failed to get QuickTweenManager when appending a tween to sequence."));
	}
	tween->SetOwner(this);

	FQuickTweenSequenceGroup& lastGroup = TweenGroups.Last();
	lastGroup.Tweens.Add(tween);
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
	TweenGroups.Add(group);
	return this;
}

void UQuickTweenSequence::Play(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;
	
	if (bIsCompleted || bIsPendingKill)
	{
		return;
	}

	bIsPlaying = true;
	for (auto [tweens] : TweenGroups)
	{
		for (UQuickTweenable* weakTween : tweens)
		{
			weakTween->Play(this);
		}
	}
}

void UQuickTweenSequence::Pause(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (bIsCompleted || bIsPendingKill)
	{
		return;
	}

	bIsPlaying = false;
	for (auto [tweens] : TweenGroups)
	{
		for (UQuickTweenable* weakTween : tweens)
		{
			weakTween->Pause(this);
		}
	}
}

void UQuickTweenSequence::Complete(UQuickTweenable* instigator, bool bSnapToEnd)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (bIsCompleted || bIsPendingKill)
	{
		return;
	}

	bIsPlaying   = false;
	bIsCompleted = true;

	ElapsedTime = GetDuration() * GetLoops();
	Progress    = 1.0f;
	CurrentLoop = Loops;

	if (CurrentTweenGroupIndex >= 0 && CurrentTweenGroupIndex < TweenGroups.Num())
	{
		auto completeGroup = [&](TArray<UQuickTweenable*>& tweens, bool toEnd, bool forceUpdate = false)
		{
			for (UQuickTweenable* tween : tweens)
			{
				if (forceUpdate)
				{
					tween->Update(0.0f, this); // force an update to ensure the tween is fully initialized
				}
				tween->Complete(this, toEnd);
			}
		};

		if (GetLoopType() == ELoopType::PingPong && GetLoops() % 2 == 0)
		{
			if (!bIsReversed)
			{
				Reverse(instigator);
			}

			for (int32 groupIdx = CurrentTweenGroupIndex; groupIdx>= 0; --groupIdx)
			{
				completeGroup(TweenGroups[groupIdx].Tweens, true);
			}
			return;
		}

		if (!bIsReversed)
		{
			for (int32 groupIdx = CurrentTweenGroupIndex; groupIdx < TweenGroups.Num(); ++groupIdx)
			{
				completeGroup(TweenGroups[groupIdx].Tweens, bSnapToEnd, true);
			}
		}
		else
		{
			for (int32 groupIdx = CurrentTweenGroupIndex; groupIdx>= 0; --groupIdx)
			{
				completeGroup(TweenGroups[groupIdx].Tweens, bSnapToEnd);
			}
		}
	}

	CurrentTweenGroupIndex = !bIsReversed ? FMath::Max(TweenGroups.Num() -1, 0) : 0;

	OnComplete.Broadcast(this);

	if (bAutoKill)
	{
		Kill(nullptr);
	}

}

void UQuickTweenSequence::Restart(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (bIsPendingKill)
	{
		UE_LOG(LogQuickTweenSequence, Warning, TEXT("Cannot restart a sequence that is pending kill."));
		return;
	}
	bIsPlaying   = true;
	bIsCompleted = false;
	bIsBackwards = false;
	bHasStarted = false;

	ElapsedTime = bIsReversed ? GetDuration() * GetLoops() : 0.0f;
	Progress    = bIsReversed ? 1.0f     : 0.0f;

	CurrentLoop = bIsReversed ? Loops : 1;
	CurrentTweenGroupIndex = bIsReversed ? TweenGroups.Num() - 1 : 0;

	for (FQuickTweenSequenceGroup& group : TweenGroups)
	{
		for (UQuickTweenable* tween : group.Tweens)
		{
			if (tween->GetIsReversed() != bIsReversed)
			{
				tween->Reverse(this);
			}
			tween->Restart(this);
		}
	}
}

void UQuickTweenSequence::Kill(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (bIsPendingKill) return;

	bIsPlaying = false;
	bIsCompleted = true;

	CurrentTweenGroupIndex = 0;

	for (FQuickTweenSequenceGroup& group : TweenGroups)
	{
		for (UQuickTweenable* tween : group.Tweens)
		{
			tween->Kill(this);
		}
	}

	bIsPendingKill = true;
	if (OnKilled.IsBound())
	{
		OnKilled.Broadcast(this);
	}
}

void UQuickTweenSequence::Reverse(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bIsReversed = !bIsReversed;
	Reverse_Tweens();

	const auto restartGroup = [&](TArray<UQuickTweenable*>& tweens)
	{
		for (UQuickTweenable* weakTween : tweens)
		{
			weakTween->Restart(this);
		}
	};

	if (bIsReversed)
	{
		for (int32 i = 0; i < CurrentTweenGroupIndex; ++i)
		{
			restartGroup(TweenGroups[i].Tweens);
		}
	}
	else
	{
		for (int32 i = CurrentTweenGroupIndex + 1; i < TweenGroups.Num(); ++i)
		{
			restartGroup(TweenGroups[i].Tweens);
		}
	}
}

void UQuickTweenSequence::Update(float deltaTime, UQuickTweenable* instigator)
{

	if (!InstigatorIsOwner(instigator) || !GetIsPlaying()) return;

	if (FMath::IsNearlyZero(GetDuration()))
	{
		Complete(instigator);
		return;
	}

	ElapsedTime += (bIsReversed ? -1.f : 1.f) * deltaTime;

	const float sequenceDuration = GetDuration();

	const int32 loop = FMath::FloorToInt(ElapsedTime / sequenceDuration);

	// ... in case deltaTime is large enough to cross multiple loops in a single update
	if (loop != CurrentLoop)
	{
		const int32 crossed = FMath::Abs(loop - CurrentLoop);
		CurrentLoop = loop;

		for (int32 i = 0; i < crossed; ++i)
		{
			OnLoop.Broadcast(this);
		}
	}

	//... check for completion
	if (Loops != INFINITE_LOOPS)
	{
		if (bIsReversed)
		{
			if (ElapsedTime < 0.f)
			{
				Complete(instigator);
				return;
			}
		}
		else
		{
			if (CurrentLoop >= Loops)
			{
				Complete(instigator);
				return;
			}
		}
	}

	float localTime = FMath::Fmod(ElapsedTime, sequenceDuration);
	if (localTime < 0.f)
	{
		localTime += sequenceDuration;
	}

	float alpha = localTime / sequenceDuration;

	if (LoopType == ELoopType::PingPong && (CurrentLoop & 1))
	{
		alpha = 1.f - alpha;
	}

	// Evaluate children
	const float SequenceTime = alpha * sequenceDuration;

	for (const FSequenceSpan& Span : Timeline)
	{
		if (!Span.Tween) continue;

		const float ChildTime =
			FMath::Clamp((SequenceTime - Span.StartTime) / Span.Duration(), 0.f, 1.f);

		Span.Tween->Evaluate(ChildTime);
	}

	OnUpdate.Broadcast(this);
}

float UQuickTweenSequence::GetDuration() const
{
	float totalDuration = 0.0f;
	for (const FQuickTweenSequenceGroup& group : TweenGroups)
	{
		float groupMaxDuration = 0.0f;
		for (const UQuickTweenable* tween : group.Tweens)
		{
			groupMaxDuration  = FMath::Max(groupMaxDuration, tween->GetDuration() * tween->GetLoops() / tween->GetTimeScale());
		}
		totalDuration += groupMaxDuration;
	}
	return totalDuration;
}

int32 UQuickTweenSequence::GetNumTweens() const
{
	uint32 count = 0;
	for (const FQuickTweenSequenceGroup& group : TweenGroups)
	{
		count += group.Tweens.Num();
	}
	return count;
}

UObject* UQuickTweenSequence::GetTween(int32 index) const
{
	int32 currentTweenIndex = 0;
	for (const FQuickTweenSequenceGroup& group : TweenGroups)
	{
		if (index < currentTweenIndex + group.Tweens.Num())
		{
			return Cast<UObject>(group.Tweens[index - currentTweenIndex]);
		}
		currentTweenIndex += group.Tweens.Num();
	}
	return nullptr;
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

void UQuickTweenSequence::HandleOnIdleTransition()
{
}

void UQuickTweenSequence::HandleOnStartTransition()
{
	ElapsedTime = bIsReversed ? GetDuration() * GetLoops() : 0.0f;
	CurrentLoop = bIsReversed ? GetLoops() - 1 : 0;
	if (OnStart.IsBound())
	{
		OnStart.Broadcast(this);
	}
	RequestStateTransition(EQuickTweenState::Play);
}

void UQuickTweenSequence::HandleOnPlayTransition()
{
}

void UQuickTweenSequence::HandleOnPauseTransition()
{
}

void UQuickTweenSequence::HandleOnCompleteTransition()
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

void UQuickTweenSequence::HandleOnKillTransition()
{
	if (OnKilled.IsBound())
	{
		OnKilled.Broadcast(this);
	}
}

void UQuickTweenSequence::RequestStateTransition(EQuickTweenState newState)
{
	if (newState == SequenceState) return;

	if (ValidTransitions[SequenceState].Contains(newState))
	{
		SequenceState = newState;
		switch (newState)
		{
		case EQuickTweenState::Idle:
			HandleOnIdleTransition();
			break;
		case EQuickTweenState::Start:
			HandleOnStartTransition();
			break;
		case EQuickTweenState::Play:
			HandleOnPlayTransition();
			break;
		case EQuickTweenState::Pause:
			HandleOnPauseTransition();
			break;
		case EQuickTweenState::Complete:
			HandleOnCompleteTransition();
			break;
		case EQuickTweenState::Kill:
			HandleOnKillTransition();
			break;
		}
	}
	else
	{
		UE_LOG(LogQuickTweenSequence, Warning, TEXT("Invalid state transition from %s to %s"),  *UEnum::GetValueAsString(SequenceState), *UEnum::GetValueAsString(newState));
	}
}
