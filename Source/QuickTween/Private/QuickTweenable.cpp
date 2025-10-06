// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickTweenable.h"


// Add default functionality here for any IQuickTweenable functions that are not pure virtual.
void IQuickTweenable::Update(float deltaTime)
{
	ensureAlwaysMsgf(false, TEXT("Update not implemented in IQuickTweenable"));
}

bool IQuickTweenable::GetIsPlaying() const
{
	ensureAlwaysMsgf(false, TEXT("GetIsPlaying not implemented in IQuickTweenable"));
	return false;
}

bool IQuickTweenable::GetIsCompleted() const
{
	ensureAlwaysMsgf(false, TEXT("GetIsCompleted not implemented in IQuickTweenable"));
	return false;
}

bool IQuickTweenable::GetIsPendingKill() const
{
	ensureAlwaysMsgf(false, TEXT("GetIsPendingKill not implemented in IQuickTweenable"));
	return false;
}

bool IQuickTweenable::GetShouldPlayWhilePaused() const
{
	ensureAlwaysMsgf(false, TEXT("GetShouldPlayWhilePaused not implemented in IQuickTweenable"));
	return false;
}
