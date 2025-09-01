// Fill out your copyright notice in the Description page of Project Settings.

#include "QuickTweenManager.h"

#include "QuickTweenable.h"

void UQuickTweenManager::OnWorldBeginPlay(UWorld& inWorld)
{
	Super::OnWorldBeginPlay(inWorld);
	TickDelegateHandler = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UQuickTweenManager::Tick), 0.0f);
}

bool UQuickTweenManager::Tick(float deltaTime)
{
	for (int i = QuickTweens.Num() - 1; i >= 0; --i)
	{
		IQuickTweenable* tweenContainer = QuickTweens[i];

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
