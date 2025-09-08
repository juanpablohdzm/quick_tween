// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickTweenSequence.h"

#include "QuickTweenManager.h"
#include "Tweens/QuickTweenBase.h"
#include "Utils/CommonValues.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenSequence, Log, All);


UQuickTweenSequence* UQuickTweenSequence::SetUp(int32 loops, ELoopType loopType, const FString& id, const UObject* worldContextObject)
{
	Loops           = loops;
	LoopType        = loopType;
	SequenceTweenId = id;

	UQuickTweenManager* manager = UQuickTweenManager::Get(worldContextObject);
	if (!manager)
	{
		UE_LOG(LogQuickTweenSequence, Log, TEXT("Failed to get QuickTweenManager for QuickTweenSequence. Tweens will not be updated."));
	}
	else
	{
		manager->AddTween(this, Badge<UQuickTweenSequence>());
	}
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

	if (UQuickTweenManager* manager = UQuickTweenManager::Get(this))
	{
		manager->RemoveTween(tween, Badge<UQuickTweenSequence>());
	}
	tween->SetIsInSequence(Badge<UQuickTweenSequence>(), true);

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

	if (UQuickTweenManager* manager = UQuickTweenManager::Get(this))
	{
		manager->RemoveTween(tween, Badge<UQuickTweenSequence>());
	}
	tween->SetIsInSequence(Badge<UQuickTweenSequence>(), true);

	FQuickTweenSequenceGroup& lastGroup = TweenGroups.Last();
	lastGroup.Tweens.Add(tween);
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Play()
{
	if (bIsCompleted || bIsPendingKill)
	{
		return this;
	}

	bIsPlaying = true;
	for (auto [tweens] : TweenGroups)
	{
		for (TWeakObjectPtr<UQuickTweenBase>& weakTween : tweens)
		{
			if (!weakTween.IsValid())
			{
				UE_LOG(LogQuickTweenSequence, Warning, TEXT("Invalid tween in sequence during Play()"));
				continue;
			}

			Badge<UQuickTweenSequence> badge;
			weakTween->Play(&badge);
		}
	}
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Pause()
{
	if (bIsCompleted || bIsPendingKill)
	{
		return this;
	}

	bIsPlaying = false;
	for (auto [tweens] : TweenGroups)
	{
		for (TWeakObjectPtr<UQuickTweenBase>& weakTween : tweens)
		{
			if (!weakTween.IsValid())
			{
				UE_LOG(LogQuickTweenSequence, Warning, TEXT("Invalid tween in sequence during Pause()"));
				continue;
			}

			Badge<UQuickTweenSequence> badge;
			weakTween->Pause(&badge);
		}
	}
	return this;
}

UQuickTweenSequence* UQuickTweenSequence::Complete()
{
	if (bIsCompleted || bIsPendingKill)
	{
		return this;
	}

	bIsPlaying   = false;
	bIsCompleted = true;

	ElapsedTime = !bIsReversed? GetDuration() * GetLoops() : 0.0f;
	Progress    = 1.0f;
	CurrentLoop = Loops;
	CurrentTweenGroupIndex = !bIsReversed ? FMath::Max(TweenGroups.Num() -1, 0) : 0;

	for (int32 groupIdx = 0; groupIdx < TweenGroups.Num(); ++groupIdx)
	{
		for (TWeakObjectPtr<UQuickTweenBase>& weakTween : TweenGroups[groupIdx].Tweens)
		{
			if (!weakTween.IsValid())
			{
				UE_LOG(LogQuickTweenSequence, Warning, TEXT("Invalid tween in sequence during Complete()"));
				continue;
			}

			Badge<UQuickTweenSequence> Badge;
			weakTween->Complete(&Badge);

		}
	}

	OnComplete.Broadcast(this);

	return this;
}


UQuickTweenSequence* UQuickTweenSequence::Restart()
{
	bIsPlaying   = true;
	bIsCompleted = false;

	ElapsedTime = bIsReversed ? GetDuration() * GetLoops() : 0.0f;
	Progress    = bIsReversed ? 1.0f     : 0.0f;

	CurrentLoop = bIsReversed ? Loops : 1;
	CurrentTweenGroupIndex = bIsReversed ? TweenGroups.Num() - 1 : 0;

	for (FQuickTweenSequenceGroup& group : TweenGroups)
	{
		for (TWeakObjectPtr<UQuickTweenBase>& tween : group.Tweens)
		{
			if (tween.IsValid())
			{
				Badge<UQuickTweenSequence> badge;
				tween->Restart(&badge);
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

	if (CurrentTweenGroupIndex < TweenGroups.Num() && CurrentTweenGroupIndex >= 0)
	{
		for (TWeakObjectPtr<UQuickTweenBase> tween : TweenGroups[CurrentTweenGroupIndex].Tweens)
		{
			Badge<UQuickTweenSequence> badge;
			tween->Stop(&badge);
		}
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
	bIsReversed = !bIsReversed;
	return Reverse_Tweens();
}

UQuickTweenSequence* UQuickTweenSequence::Reverse_Tweens()
{
	for (auto [tweens] : TweenGroups)
	{
		for (TWeakObjectPtr<UQuickTweenBase>& weakTween : tweens)
		{
			if (!weakTween.IsValid())
			{
				UE_LOG(LogQuickTweenSequence, Warning, TEXT("Invalid tween in sequence during Reverse()"));
				continue;
			}

			Badge<UQuickTweenSequence> badge;
			weakTween->Reverse(&badge);
		}
	}
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

	switch (LoopType)
	{
		case ELoopType::Restart:
			Update_Restart(deltaTime);
			break;
		case ELoopType::PingPong:
			Update_PingPong(deltaTime);
			break;
	}
}

void UQuickTweenSequence::Update_Restart(float deltaTime)
{
	ElapsedTime = !bIsReversed ?  ElapsedTime + deltaTime : ElapsedTime - deltaTime;

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}

	bool shouldComplete = false;
	if (!bIsReversed)
	{
		CurrentLoop = (ElapsedTime / GetDuration()) + 1;
		shouldComplete = CurrentLoop > Loops;
	}
	else
	{
		const float mod = FMath::Fmod(ElapsedTime, GetDuration());
		const uint32 loop = ElapsedTime / GetDuration();
		CurrentLoop = loop + 1;
		shouldComplete = loop <= 0 && FMath::IsNearlyZero(mod);
	}

	if (Loops != INFINITE_LOOPS && shouldComplete)
	{
		Complete();
		return;
	}


	bool bShouldCompleteLoop = CurrentTweenGroupIndex >= TweenGroups.Num() || CurrentTweenGroupIndex < 0;

	if (!bShouldCompleteLoop)
	{
		const FQuickTweenSequenceGroup& currentGroup = TweenGroups[CurrentTweenGroupIndex];
		uint32 completedTweens = 0;
		for (TWeakObjectPtr<UQuickTweenBase> tween : currentGroup.Tweens)
		{
			if (tween.IsValid())
			{
				Badge<UQuickTweenSequence> badge;
				tween->Update(deltaTime, &badge);

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
		bShouldCompleteLoop = completedTweens == currentGroup.Tweens.Num();
	}

	if (bShouldCompleteLoop)
	{
		if (bIsReversed)
		{
			CurrentTweenGroupIndex--;
		}else
		{
			CurrentTweenGroupIndex++;
		}
		if (CurrentTweenGroupIndex >= TweenGroups.Num() || CurrentTweenGroupIndex < 0)
		{
			CurrentTweenGroupIndex = bIsReversed ? TweenGroups.Num() - 1 : 0;
			for (auto [tweens] : TweenGroups)
			{
				for (TWeakObjectPtr<UQuickTweenBase> tween : tweens)
				{
					if (tween.IsValid())
					{
						Badge<UQuickTweenSequence> badge;
						tween->Restart(&badge);
					}
					else
					{
						UE_LOG(LogQuickTweenSequence, Warning, TEXT("A tween in the sequence is invalid. This should not happen."));
					}
				}
			}
			CurrentLoop = bIsReversed ? CurrentLoop - 1 : CurrentLoop + 1;
		}
	}
}

void UQuickTweenSequence::Update_PingPong(float deltaTime)
{
	ElapsedTime = !bIsReversed ?  ElapsedTime + deltaTime : ElapsedTime - deltaTime;

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}

	bool bShouldCompleteLoop = CurrentTweenGroupIndex >= TweenGroups.Num() || CurrentTweenGroupIndex < 0;

	if (!bShouldCompleteLoop)
	{
		const FQuickTweenSequenceGroup& currentGroup = TweenGroups[CurrentTweenGroupIndex];
		uint32 completedTweens = 0;
		for (TWeakObjectPtr<UQuickTweenBase> tween : currentGroup.Tweens)
		{
			if (tween.IsValid())
			{
				Badge<UQuickTweenSequence> badge;
				tween->Update(deltaTime, &badge);

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
		bShouldCompleteLoop = completedTweens == currentGroup.Tweens.Num();
	}

	if (bShouldCompleteLoop)
	{
		if (bIsReversed)
		{
			CurrentTweenGroupIndex--;
		}else
		{
			CurrentTweenGroupIndex++;
		}
		if (CurrentTweenGroupIndex >= TweenGroups.Num() || CurrentTweenGroupIndex < 0)
		{
			CurrentLoop = bIsReversed ? CurrentLoop - 1 : CurrentLoop + 1;
			if (Loops != INFINITE_LOOPS && (!bIsReversed && CurrentLoop > Loops || bIsReversed && (CurrentLoop - 1) <= 0))
			{
				Complete();
				return;
			}

			CurrentTweenGroupIndex = bIsReversed ? TweenGroups.Num() - 1 : 0;
			Reverse_Tweens();
			for (auto [tweens] : TweenGroups)
			{
				for (TWeakObjectPtr<UQuickTweenBase> tween : tweens)
				{
					if (tween.IsValid())
					{
						Badge<UQuickTweenSequence> badge;
						tween->Restart(&badge);
					}
					else
					{
						UE_LOG(LogQuickTweenSequence, Warning, TEXT("A tween in the sequence is invalid. This should not happen."));
					}
				}
			}
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
	int32 currentTweenIndex = 0;
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
