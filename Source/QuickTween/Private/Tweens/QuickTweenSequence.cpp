// Fill out your copyright notice in the Description page of Project Settings.


#include "Tweens/QuickTweenSequence.h"

#include "QuickTweenManager.h"
#include "Tweens/QuickTweenBase.h"
#include "Utils/CommonValues.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenSequence, Log, All);


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
	return;
}

void UQuickTweenSequence::Join(UQuickTweenable* tween)
{
	ensureAlwaysMsgf(tween, TEXT("Join called with a null tween. This should never happen."));
	if (tween->GetLoops()  == INFINITE_LOOPS)
	{
		UE_LOG(LogQuickTweenSequence, Warning, TEXT("Joining a tween with infinite loops is not allowed. Please set a finite number of loops."));
		return;
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

	FQuickTweenSequenceGroup group;
	group.Tweens.Add(tween);
	TweenGroups.Add(group);
}

void UQuickTweenSequence::Append(UQuickTweenable* tween)
{
	ensureAlwaysMsgf(tween, TEXT("Append called with a null tween. This should never happen."));
	if (tween->GetLoops()  == INFINITE_LOOPS)
	{
		UE_LOG(LogQuickTweenSequence, Warning, TEXT("Joining a tween with infinite loops is not allowed. Please set a finite number of loops."));
		return;
	}

	if (TweenGroups.Num() == 0)
	{
		return Join(tween);
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

void UQuickTweenSequence::Stop(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (CurrentTweenGroupIndex < TweenGroups.Num() && CurrentTweenGroupIndex >= 0)
	{
		for (UQuickTweenable* tween : TweenGroups[CurrentTweenGroupIndex].Tweens)
		{
			tween->Stop(this);
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


	if (bAutoKill)
	{
		Kill(nullptr);
	}

	OnComplete.Broadcast(this);
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

void UQuickTweenSequence::Reset(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;


	ElapsedTime = 0.0f;
	Progress    = 0.0f;
	bIsPlaying  = false;
	bIsCompleted= false;
	bIsBackwards= false;
	bIsReversed = false;
	Loops       = 0;
	LoopType    = ELoopType::Restart;
	CurrentLoop = 1;
	CurrentTweenGroupIndex = 0;
	TweenGroups.Empty();
}

void UQuickTweenSequence::Kill(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	if (bIsPendingKill) return;

	bIsPlaying = false;
	bIsCompleted = true;

	Stop(instigator);
	CurrentTweenGroupIndex = 0;

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

void UQuickTweenSequence::Reverse_Tweens()
{
	for (auto [tweens] : TweenGroups)
	{
		for (UQuickTweenable* weakTween : tweens)
		{
			weakTween->Reverse(this);
		}
	}
}

void UQuickTweenSequence::Update(float deltaTime, UQuickTweenable* instigator)
{

	if (!InstigatorIsOwner(instigator)) return;

	if (GetIsCompleted() || !GetIsPlaying()) return;

	switch (LoopType)
	{
		case ELoopType::Restart:
			Update_Restart(deltaTime, instigator);
			break;
		case ELoopType::PingPong:
			Update_PingPong(deltaTime, instigator);
			break;
	}
}

void UQuickTweenSequence::SetAutoKill(bool bShouldAutoKill, UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bAutoKill = bShouldAutoKill;
	if (GetIsCompleted())
	{
		bIsPendingKill = bAutoKill;
	}
}

void UQuickTweenSequence::Update_Restart(float deltaTime, UQuickTweenable* instigator)
{
	ElapsedTime = !bIsReversed ?  ElapsedTime + deltaTime : ElapsedTime - deltaTime;

	if (OnUpdate.IsBound())
	{
		OnUpdate.Broadcast(this);
	}

	bool shouldComplete;
	if (!bIsReversed)
	{
		CurrentLoop = (ElapsedTime / GetDuration()) + 1;
		shouldComplete = CurrentLoop > Loops;
	}
	else
	{
		if (ElapsedTime < 0.0f)
		{
			shouldComplete = true;
		}
		else
		{
			const float mod = FMath::Fmod(ElapsedTime, GetDuration());
			const uint32 loop = ElapsedTime / GetDuration();
			CurrentLoop = loop + 1;
			shouldComplete = loop <= 0 && FMath::IsNearlyZero(mod);
		}
	}

	if (Loops != INFINITE_LOOPS && shouldComplete)
	{
		Complete(instigator);
		return;
	}


	bool bShouldCompleteLoop = CurrentTweenGroupIndex >= TweenGroups.Num() || CurrentTweenGroupIndex < 0;

	if (!bShouldCompleteLoop)
	{
		const FQuickTweenSequenceGroup& currentGroup = TweenGroups[CurrentTweenGroupIndex];
		uint32 completedTweens = 0;
		for (UQuickTweenable* tween : currentGroup.Tweens)
		{
			tween->Update(deltaTime, this);

			if (tween->GetIsCompleted())
			{
				completedTweens++;
			}
		}
		bShouldCompleteLoop = completedTweens == currentGroup.Tweens.Num();
	}

	if (bShouldCompleteLoop)
	{
		CurrentTweenGroupIndex += bIsReversed ? -1 : 1;

		if (CurrentTweenGroupIndex >= TweenGroups.Num() || CurrentTweenGroupIndex < 0)
		{
			CurrentTweenGroupIndex = bIsReversed ? TweenGroups.Num() - 1 : 0;
			for (auto [tweens] : TweenGroups)
			{
				for (UQuickTweenable* tween : tweens)
				{
					tween->Restart(this);
				}
			}
			CurrentLoop = bIsReversed ? CurrentLoop - 1 : CurrentLoop + 1;
		}
	}
}

void UQuickTweenSequence::Update_PingPong(float deltaTime, UQuickTweenable* instigator)
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
		for (UQuickTweenable* tween : currentGroup.Tweens)
		{
			tween->Update(deltaTime, this);

			if (tween->GetIsCompleted())
			{
				completedTweens++;
			}
		}
		bShouldCompleteLoop = completedTweens == currentGroup.Tweens.Num();
	}

	if (bShouldCompleteLoop)
	{
		const int32 Negate = bIsReversed ? -1 : 1;
		CurrentTweenGroupIndex += bIsBackwards ? -1 * Negate : 1 * Negate;

		if (CurrentTweenGroupIndex >= TweenGroups.Num() || CurrentTweenGroupIndex < 0)
		{
			CurrentLoop = bIsReversed ? CurrentLoop - 1 : CurrentLoop + 1;
			if (Loops != INFINITE_LOOPS && (!bIsReversed && CurrentLoop > Loops || bIsReversed && (CurrentLoop - 1) <= 0))
			{
				Complete(instigator);
				return;
			}

			for (auto [tweens] : TweenGroups)
			{
				for (UQuickTweenable* tween : tweens)
				{
					if (!(tween->GetLoopType() == ELoopType::PingPong && tween->GetLoops() % 2 == 0))
					{
						tween->Reverse(this);
					}
					tween->Restart(this);
				}
			}

			bIsBackwards = !bIsBackwards;
			CurrentTweenGroupIndex = bIsBackwards ? TweenGroups.Num() - 1 : 0;
		}
	}
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
