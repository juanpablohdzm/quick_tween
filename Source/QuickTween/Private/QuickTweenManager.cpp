// Fill out your copyright notice in the Description page of Project Settings.

#include "QuickTweenManager.h"
#include "QuickTweenable.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenManager, Log, All);

UQuickTweenManager* UQuickTweenManager::Get(const UObject* worldContextObject)
{
	if (worldContextObject)
	{
		if (UWorld* world = worldContextObject->GetWorld())
		{
			return world->GetSubsystem<UQuickTweenManager>();
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
	check(!bIsInitialized)
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

	UE_LOG(LogQuickTweenManager, Verbose, TEXT("QuickTweenManager Tick called with deltaTime: %f"), deltaTime);
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
	UE_LOG(LogQuickTweenManager, Log, TEXT("Adding tween from QuickTweenManager"));
	QuickTweens.Add(tween);
}

void UQuickTweenManager::RemoveTween(class UQuickTweenable* tween)
{
	UE_LOG(LogQuickTweenManager, Log, TEXT("Remove tween from QuickTweenManager"));
	QuickTweens.RemoveSingleSwap(tween);
}
