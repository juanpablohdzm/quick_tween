// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickTweenSequence.h"

#include "Tweens/QuickTweenBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenSequence, Log, All);

UQuickTweenSequence* UQuickTweenSequence::Play()
{
	bIsPlaying = true;
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Stop()
{
	bIsCompleted = true;
	bIsPlaying = false;
	for (FQuickTweenSequenceGroup& group : TweenGroups)
	{
		for (TWeakObjectPtr<UQuickTweenBase>& tween : group.Tweens)
		{
			if (tween.IsValid())
			{
				tween->Stop();
			}
		}
	}
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Pause()
{
	bIsPlaying = false;
	return this;
}

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


void UQuickTweenSequence::Update(float deltaTime)
{
	if (!bIsPlaying || bIsCompleted)
	{
		return;
	}

	const FQuickTweenSequenceGroup& currentGroup = TweenGroups[currentTweenIndex];
	uint32 completedTweens = 0;
	for (TWeakObjectPtr<UQuickTweenBase> tween : currentGroup.Tweens)
	{
		if (tween.IsValid())
		{
			if (!tween->Update(deltaTime))
			{
				if (tween->GetIsCompleted())
				{
					completedTweens++;
				}
				else
				{
					UE_LOG(LogQuickTweenSequence, Warning, TEXT("Tween in sequence can not update but is not completed, considering paused.... pausing all sequence"));
					Pause();
					return;
				}
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
		currentTweenIndex++;
		if (currentTweenIndex >= TweenGroups.Num())
		{
			Complete();
		}
	}
}

UQuickTweenSequence* UQuickTweenSequence::Complete()
{
	Stop();
	return this;
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
