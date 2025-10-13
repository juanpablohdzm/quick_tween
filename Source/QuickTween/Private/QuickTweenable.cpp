// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickTweenable.h"


// Add default functionality here for any IQuickTweenable functions that are not pure virtual.
void IQuickTweenable::Update(float deltaTime, UObject* instigator)
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

void IQuickTweenable::SetOwner(UObject* owner)
{
	ensureAlwaysMsgf(false, TEXT("SetOwner not implemented in IQuickTweenable"));
}

void IQuickTweenable::Play(UObject* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Play not implemented in IQuickTweenable"));
}

void IQuickTweenable::Pause(UObject* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Pause not implemented in IQuickTweenable"));
}

void IQuickTweenable::Stop(UObject* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Stop not implemented in IQuickTweenable"));
}

void IQuickTweenable::Reverse(UObject* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Reverse not implemented in IQuickTweenable"));
}

void IQuickTweenable::Restart(UObject* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Restart not implemented in IQuickTweenable"));
}

void IQuickTweenable::Complete(UObject* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Complete not implemented in IQuickTweenable"));
}

void IQuickTweenable::Reset(UObject* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Reset not implemented in IQuickTweenable"));
}

void IQuickTweenable::Kill(UObject* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Kill not implemented in IQuickTweenable"));
}

void IQuickTweenable::SetAutoKill(bool bShouldAutoKill, UObject* instigator)
{
	ensureAlwaysMsgf(false, TEXT("SetAutoKill not implemented in IQuickTweenable"));
}

float IQuickTweenable::GetDuration() const
{
	ensureAlwaysMsgf(false, TEXT("GetDuration not implemented in IQuickTweenable"));
	return 0.0f;
}

float IQuickTweenable::GetElapsedTime() const
{
	ensureAlwaysMsgf(false, TEXT("GetElapsedTime not implemented in IQuickTweenable"));
	return 0.0f;
}

float IQuickTweenable::GetProgress() const
{
	ensureAlwaysMsgf(false, TEXT("GetProgress not implemented in IQuickTweenable"));
	return 0.0f;
}

float IQuickTweenable::GetTimeScale() const
{
	ensureAlwaysMsgf(false, TEXT("GetTimeScale not implemented in IQuickTweenable"));
	return 0.0f;
}

bool IQuickTweenable::GetIsBackwards() const
{
	ensureAlwaysMsgf(false, TEXT("GetIsBackwards not implemented in IQuickTweenable"));
	return false;
}

bool IQuickTweenable::GetIsReversed() const
{
	ensureAlwaysMsgf(false, TEXT("GetIsReversed not implemented in IQuickTweenable"));
	return false;
}

EEaseType IQuickTweenable::GetEaseType() const
{
	ensureAlwaysMsgf(false, TEXT("GetEaseType not implemented in IQuickTweenable"));
	return static_cast<EEaseType>(0);
}

UCurveFloat* IQuickTweenable::GetEaseCurve() const
{
	ensureAlwaysMsgf(false, TEXT("GetEaseCurve not implemented in IQuickTweenable"));
	return nullptr;
}

int32 IQuickTweenable::GetLoops() const
{
	ensureAlwaysMsgf(false, TEXT("GetLoops not implemented in IQuickTweenable"));
	return 0;
}

ELoopType IQuickTweenable::GetLoopType() const
{
	ensureAlwaysMsgf(false, TEXT("GetLoopType not implemented in IQuickTweenable"));
	return static_cast<ELoopType>(0);
}

FString IQuickTweenable::GetTweenTag() const
{
	ensureAlwaysMsgf(false, TEXT("GetTweenTag not implemented in IQuickTweenable"));
	return FString();
}

int32 IQuickTweenable::GetCurrentLoop() const
{
	ensureAlwaysMsgf(false, TEXT("GetCurrentLoop not implemented in IQuickTweenable"));
	return 0;
}

bool IQuickTweenable::GetAutoKill() const
{
	ensureAlwaysMsgf(false, TEXT("GetAutoKill not implemented in IQuickTweenable"));
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
