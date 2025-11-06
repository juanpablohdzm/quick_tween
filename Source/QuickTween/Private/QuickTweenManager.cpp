// Fill out your copyright notice in the Description page of Project Settings.

#include "QuickTweenManager.h"
#include "QuickTweenable.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenManager, Log, All);

UQuickTweenManager* UQuickTweenManager::Get(const UObject* worldContextObject)
{
	if (UWorld* world = GEngine->GetWorldFromContextObject(worldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		return world->GetSubsystem<UQuickTweenManager>();
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
		IQuickTweenable* tweenContainer = reinterpret_cast<IQuickTweenable*>(QuickTweens[i]);
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

void UQuickTweenManager::AddTween(IQuickTweenable* tween)
{
	UE_LOG(LogQuickTweenManager, Log, TEXT("Adding sequence to QuickTweenManager"));
	UObject* obj = reinterpret_cast<UObject*>(tween);
	QuickTweens.Add(obj);
}

void UQuickTweenManager::RemoveTween(class IQuickTweenable* tween)
{
	UE_LOG(LogQuickTweenManager, Verbose, TEXT("Remove sequence to QuickTweenManager"));
	UObject* obj = reinterpret_cast<UObject*>(tween);
	if (obj && QuickTweens.Find(obj) != INDEX_NONE)
	{
		QuickTweens.Remove(obj);
	}
}
