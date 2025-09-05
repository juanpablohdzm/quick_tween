// Fill out your copyright notice in the Description page of Project Settings.

#include "QuickTweenManager.h"
#include "QuickTweenable.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenManager, Log, All);

UQuickTweenManager* UQuickTweenManager::Get(const UObject* worldContextObject)
{
	if (UWorld* world = GEngine->GetWorldFromContextObject(worldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		return world->GetSubsystem<UQuickTweenManager>();
	}
	return nullptr;
}

void UQuickTweenManager::OnWorldBeginPlay(UWorld& inWorld)
{
	Super::OnWorldBeginPlay(inWorld);
	TickDelegateHandler = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UQuickTweenManager::Tick), 0.0f);
}

bool UQuickTweenManager::Tick(float deltaTime)
{
	for (int i = QuickTweens.Num() - 1; i >= 0; --i)
	{
		IQuickTweenable* tweenContainer = reinterpret_cast<IQuickTweenable*>(QuickTweens[i]);

		if (tweenContainer->GetIsPlaying())
		{
			tweenContainer->Update(deltaTime);
		}

		if (tweenContainer->GetIsPendingKill())
		{
			QuickTweens.RemoveAt(i);
			continue;
		}
	}
	return true;
}

UQuickTweenManager::~UQuickTweenManager()
{
	FTSTicker::RemoveTicker(TickDelegateHandler);
}

void UQuickTweenManager::AddTween(IQuickTweenable* tween, Badge<UQuickTweenBase> badge)
{
	UE_LOG(LogQuickTweenManager, Verbose, TEXT("Adding tween to QuickTweenManager"));
	UObject* obj = reinterpret_cast<UObject*>(tween);
	QuickTweens.Add(obj);
}

void UQuickTweenManager::AddTween(IQuickTweenable* tween, Badge<UQuickTweenSequence> badge)
{
	UE_LOG(LogQuickTweenManager, Verbose, TEXT("Adding tween to QuickTweenManager"));
	UObject* obj = reinterpret_cast<UObject*>(tween);
	QuickTweens.Add(obj);
}

void UQuickTweenManager::RemoveTween(class IQuickTweenable* tween, Badge<class UQuickTweenBase> badge)
{
	UE_LOG(LogQuickTweenManager, Verbose, TEXT("Remove tween to QuickTweenManager"));
	UObject* obj = reinterpret_cast<UObject*>(tween);
	if (obj && QuickTweens.Find(obj) != INDEX_NONE)
	{
		QuickTweens.Remove(obj);
	}
}

void UQuickTweenManager::RemoveTween(class IQuickTweenable* tween, Badge<class UQuickTweenSequence> badge)
{
	UE_LOG(LogQuickTweenManager, Verbose, TEXT("Remove tween to QuickTweenManager"));
	UObject* obj = reinterpret_cast<UObject*>(tween);
	if (obj && QuickTweens.Find(obj) != INDEX_NONE)
	{
		QuickTweens.Remove(obj);
	}
}
