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

void UQuickTweenSequence::Play(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;
	
	if (SequenceState == EQuickTweenState::Idle)
	{
		RequestStateTransition(EQuickTweenState::Start);
	}
	else
	{
		RequestStateTransition(EQuickTweenState::Play);
	}
}

void UQuickTweenSequence::Pause(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	RequestStateTransition(EQuickTweenState::Pause);
}

void UQuickTweenSequence::Complete(UQuickTweenable* instigator, bool bSnapToEnd)
{
	if (!InstigatorIsOwner(instigator)) return;

	RequestStateTransition(EQuickTweenState::Complete, bSnapToEnd);
}

void UQuickTweenSequence::Restart(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	RequestStateTransition(EQuickTweenState::Idle);
}

void UQuickTweenSequence::Kill(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	RequestStateTransition(EQuickTweenState::Kill);
}

void UQuickTweenSequence::Reverse(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bIsReversed = !bIsReversed;
}

void UQuickTweenSequence::Update(float deltaTime, UQuickTweenable* instigator)
{

	if (!InstigatorIsOwner(instigator) || !GetIsPlaying()) return;

	if (FMath::IsNearlyZero(GetLoopDuration()))
	{
		Complete(instigator);
		return;
	}

	ElapsedTime += (bIsReversed ? -1.f : 1.f) * deltaTime;

	const float loopDuration = GetLoopDuration();

	const int32 loop = FMath::FloorToInt(ElapsedTime / loopDuration);

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

	float localTime = FMath::Fmod(ElapsedTime, loopDuration);
	if (localTime < 0.f)
	{
		localTime += loopDuration;
	}

	float alpha = localTime / loopDuration;
	if (LoopType == ELoopType::PingPong && (CurrentLoop & 1))
	{
		alpha = 1.f - alpha;
	}

	const float sequenceTime = alpha * loopDuration;

	for (const FQuickTweenSequenceGroup& group : TweenGroups)
	{
		if (sequenceTime >= group.StartTime && sequenceTime < group.StartTime + group.Duration)
		{
			for (UQuickTweenable* tween : group.Tweens)
			{
				const float childTime = (sequenceTime - group.StartTime) / tween->GetTotalDuration(); // ... allow overflow to let the tween handle it

				tween->Evaluate(childTime);
			}
			break;
		}
	}

	OnUpdate.Broadcast(this);
}

float UQuickTweenSequence::GetLoopDuration() const
{
	float totalDuration = 0.0f;
	for (const FQuickTweenSequenceGroup& group : TweenGroups)
	{
		float groupMaxDuration = 0.0f;
		for (const UQuickTweenable* tween : group.Tweens)
		{
			groupMaxDuration  = FMath::Max(groupMaxDuration, tween->GetTotalDuration());
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
	ElapsedTime = bIsReversed ? GetTotalDuration() : 0.0f;
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

void UQuickTweenSequence::HandleOnCompleteTransition(bool bSnapToEnd)
{
	ElapsedTime = bIsReversed ? 0.0f : GetTotalDuration();

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
