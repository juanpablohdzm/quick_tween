// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#include "QuickTweenManager.h"
#include "QuickTweenable.h"
#include "Algo/AllOf.h"
#include "Algo/Find.h"
#include "Algo/FindLast.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenManager, Log, All);

UQuickTweenManager* UQuickTweenManager::Get(const UObject* worldContextObject)
{
	if (IsValid(worldContextObject))
	{
		if (UWorld* world = worldContextObject->GetWorld())
		{
			UQuickTweenManager* subsystem =  world->GetSubsystem<UQuickTweenManager>();
			return subsystem;
		}
	}
	return nullptr;
}

UWorld* UQuickTweenManager::GetTickableGameObjectWorld() const
{
	return GetWorld();
}

void UQuickTweenManager::Initialize(FSubsystemCollectionBase& Collection)
{
	check(!bIsInitialized);
	bIsInitialized = true;

	// Refresh the tick type after initialization
	SetTickableTickType(GetTickableTickType());
}

void UQuickTweenManager::Deinitialize()
{
	check(bIsInitialized);
	bIsInitialized = false;

	// Always cancel tick as this is about to be destroyed
	SetTickableTickType(ETickableTickType::Never);
}

void UQuickTweenManager::Tick(float deltaTime)
{
	if (!bIsInitialized)
	{
		return;
	}

	for (int i = QuickTweens.Num() - 1; i >= 0; --i)
	{
		UQuickTweenable* tweenContainer = QuickTweens[i];
		if (!tweenContainer)
		{
			UE_LOG(LogQuickTweenManager, Warning, TEXT("Null tween found in QuickTweenManager, removing it."));
			QuickTweens.RemoveAt(i);
			continue;
		}

		if (tweenContainer->GetIsPendingKill())
		{
			QuickTweens.RemoveAt(i);
			continue;
		}

		if (GetWorld()->IsPaused() && !tweenContainer->GetShouldPlayWhilePaused())
		{
			continue;
		}

		if (tweenContainer->GetIsPlaying())
		{
			tweenContainer->Update(deltaTime);
		}

	}
}

TStatId UQuickTweenManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UQuickTweenManager, STATGROUP_Tickables);
}

void UQuickTweenManager::AddTween(UQuickTweenable* tween)
{
	QuickTweens.Add(tween);
}

void UQuickTweenManager::RemoveTween(class UQuickTweenable* tween)
{
	QuickTweens.RemoveSingleSwap(tween);
}

UQuickTweenable* UQuickTweenManager::FindTweenByPredicate(TFunctionRef<bool(UQuickTweenable*)> predicate) const
{
	UQuickTweenable* const* const ptr = Algo::FindByPredicate(QuickTweens, predicate);
	return ptr ? *ptr : nullptr;
}

UQuickTweenable* UQuickTweenManager::FindLastTweenByPredicate(TFunctionRef<bool(UQuickTweenable*)> predicate) const
{
	UQuickTweenable* const* const ptr = Algo::FindLastByPredicate(QuickTweens, predicate);
	return ptr ? *ptr : nullptr;
}

TArray<UQuickTweenable*> UQuickTweenManager::FindAllTweensByPredicate(TFunctionRef<bool(UQuickTweenable*)> predicate) const
{
	TArray<UQuickTweenable*> results;
	for (UQuickTweenable* tween : QuickTweens)
	{
		if (predicate(tween))
		{
			results.Add(tween);
		}
	}
	return results;
}

void UQuickTweenManager::ExecutePredicateByCondition(
	TFunctionRef<void(UQuickTweenable*)> action,
	TFunctionRef<bool(const UQuickTweenable*)> predicate) const
{
	for (UQuickTweenable* tween : QuickTweens)
	{
		if (predicate(tween))
		{
			action(tween);
		}
	}
}
