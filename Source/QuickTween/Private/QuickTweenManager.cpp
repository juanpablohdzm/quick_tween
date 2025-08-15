// Fill out your copyright notice in the Description page of Project Settings.

#include "QuickTweenManager.h"

void UQuickTweenManager::OnWorldBeginPlay(UWorld& inWorld)
{
	Super::OnWorldBeginPlay(inWorld);
	TickDelegateHandler = FTSTicker::AddTicker(FTickerDelegate::CreateUObject(this, &UQuickTweenManager::Tick), 0.0f);
}

bool UQuickTweenManager::Tick(float deltaTime)
{
	return true; // Return true to keep the ticker active
}

UQuickTweenManager::~UQuickTweenManager()
{
	FTSTicker::RemoveTicker(TickDelegateHandler);
}
