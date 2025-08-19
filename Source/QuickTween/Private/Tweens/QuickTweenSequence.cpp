// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickTweenSequence.h"
#include "Tweens/QuickTweenBase.h"
#include "Utils/CommonValues.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenSequence, Log, All);

UQuickTweenSequence* UQuickTweenSequence::Join(UQuickTweenBase* tween)
{
	ensureAlwaysMsgf(tween, TEXT("Join called with a null tween. This should never happen."));
	if (tween->GetLoops()  == INFINITE_LOOPS)
	{
		UE_LOG(LogQuickTweenSequence, Warning, TEXT("Joining a tween with infinite loops is not allowed. Please set a finite number of loops."));
		return this;
	}

	FQuickTweenSequenceGroup group;
	group.Tweens.Add(tween);
	TweenGroups.Add(group);
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Append(UQuickTweenBase* tween)
{
	ensureAlwaysMsgf(tween, TEXT("Append called with a null tween. This should never happen."));
	if (tween->GetLoops()  == INFINITE_LOOPS)
	{
		UE_LOG(LogQuickTweenSequence, Warning, TEXT("Joining a tween with infinite loops is not allowed. Please set a finite number of loops."));
		return this;
	}

	if (TweenGroups.Num() == 0)
	{
		return Join(tween);
	}
	FQuickTweenSequenceGroup& lastGroup = TweenGroups.Last();
	lastGroup.Tweens.Add(tween);
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Play()
{
	bIsPlaying = true;
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Pause()
{
	bIsPlaying = false;
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Complete()
{
	bIsCompleted = true;
	bIsPlaying = false;
	ensureAlways(false, TEXT("Completing a sequence is not implemented yet. This should be handled by the caller."));
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Restart()
{
	bIsPlaying = true;
	bIsCompleted = false;
	CurrentTweenGroupIndex = 0;

	for (FQuickTweenSequenceGroup& group : TweenGroups)
	{
		for (TWeakObjectPtr<UQuickTweenBase>& tween : group.Tweens)
		{
			if (tween.IsValid())
			{
				tween->Restart();
			}
			else
			{
				UE_LOG(LogQuickTweenSequence, Warning, TEXT("A tween in the sequence is invalid during restart. This should not happen."));
			}
		}
	}

	return this;
}

UQuickTweenSequence* UQuickTweenSequence::KillSequence()
{
	bIsPlaying = false;
	bIsCompleted = true;
	for (TWeakObjectPtr<UQuickTweenBase> tween : TweenGroups[CurrentTweenGroupIndex].Tweens)
	{
		tween->Stop();
	}
	CurrentTweenGroupIndex = 0;

	bIsPendingKill = true;
	if (OnKilled.IsBound())
	{
		OnKilled.Broadcast(this);
	}
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Reverse()
{
	bIsBackwards = !bIsBackwards;
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::TogglePause()
{
	if (bIsPlaying)
	{
		Pause();
	}
	else
	{
		Play();
	}
	return this;
}

void UQuickTweenSequence::Update(float deltaTime)
{
	if (GetIsCompleted() || !GetIsPlaying()) return;

	ElapsedTime += deltaTime;

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}


	if (Loops != INFINITE_LOOPS && CurrentLoop >= Loops)
	{
		Complete();
		return;
	}


	const FQuickTweenSequenceGroup& currentGroup = TweenGroups[CurrentTweenGroupIndex];
	uint32 completedTweens = 0;
	for (TWeakObjectPtr<UQuickTweenBase> tween : currentGroup.Tweens)
	{
		if (tween.IsValid())
		{
			tween->Update(deltaTime);

			if (tween->GetIsCompleted())
			{
				completedTweens++;
			}
		}
		else
		{
			UE_LOG(LogQuickTweenSequence, Warning, TEXT("A tween in the sequence is invalid. This should not happen. Considering it as completed."));
			completedTweens++;
		}
	}

	if (completedTweens == currentGroup.Tweens.Num())
	{
		if (bIsBackwards)
		{
			CurrentTweenGroupIndex--;
		}else
		{
			CurrentTweenGroupIndex++;
		}
		if (CurrentTweenGroupIndex >= TweenGroups.Num() || CurrentTweenGroupIndex < 0)
		{
			switch (LoopType)
			{
			case ELoopType::Restart:
				CurrentTweenGroupIndex = bIsBackwards ? TweenGroups.Num() - 1 : 0;
				break;
			case ELoopType::PingPong:
				Reverse();
				break;
			default:
				ensureAlwaysMsgf(false, TEXT("LoopType %s is not implemented in UQuickTweenBase::Update"), *UEnum::GetValueAsString(LoopType));
			}
			CurrentLoop++;
		}
	}
}

float UQuickTweenSequence::GetDuration() const
{
	float totalDuration = 0.0f;
	for (const FQuickTweenSequenceGroup& group : TweenGroups)
	{
		float groupMaxDuration = 0.0f;
		for (const TWeakObjectPtr<UQuickTweenBase>& tween : group.Tweens)
		{
			if (tween.IsValid())
			{
				groupMaxDuration  = FMath::Max(groupMaxDuration, tween->GetDuration() * tween->GetLoops());
			}
			else
			{
				UE_LOG(LogQuickTweenSequence, Warning, TEXT("A tween in the sequence is invalid. This should not happen."));
			}
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

UQuickTweenBase* UQuickTweenSequence::GetTween(int32 index) const
{
	uint32 currentTweenIndex = 0;
	for (const FQuickTweenSequenceGroup& group : TweenGroups)
	{
		if (index < currentTweenIndex + group.Tweens.Num())
		{
			return group.Tweens[index - currentTweenIndex].Get();
		}
		currentTweenIndex += group.Tweens.Num();
	}
	return nullptr;
}
