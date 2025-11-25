// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickTweenable.h"


// Add default functionality here for any IQuickTweenable functions that are not pure virtual.
void UQuickTweenable::Update(float deltaTime, UQuickTweenable* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Update not implemented in IQuickTweenable"));
}

bool UQuickTweenable::GetIsPlaying() const
{
	ensureAlwaysMsgf(false, TEXT("GetIsPlaying not implemented in IQuickTweenable"));
	return false;
}

bool UQuickTweenable::GetIsCompleted() const
{
	ensureAlwaysMsgf(false, TEXT("GetIsCompleted not implemented in IQuickTweenable"));
	return false;
}

void UQuickTweenable::SetOwner(UQuickTweenable* owner)
{
	ensureAlwaysMsgf(false, TEXT("SetOwner not implemented in IQuickTweenable"));
}

void UQuickTweenable::Play(UQuickTweenable* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Play not implemented in IQuickTweenable"));
}

void UQuickTweenable::Pause(UQuickTweenable* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Pause not implemented in IQuickTweenable"));
}

void UQuickTweenable::Stop(UQuickTweenable* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Stop not implemented in IQuickTweenable"));
}

void UQuickTweenable::Reverse(UQuickTweenable* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Reverse not implemented in IQuickTweenable"));
}

void UQuickTweenable::Restart(UQuickTweenable* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Restart not implemented in IQuickTweenable"));
}

void UQuickTweenable::Complete(UQuickTweenable* instigator, bool bSnapToEnd)
{
	ensureAlwaysMsgf(false, TEXT("Complete not implemented in IQuickTweenable"));
}

void UQuickTweenable::Reset(UQuickTweenable* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Reset not implemented in IQuickTweenable"));
}

void UQuickTweenable::Kill(UQuickTweenable* instigator)
{
	ensureAlwaysMsgf(false, TEXT("Kill not implemented in IQuickTweenable"));
}

void UQuickTweenable::SetAutoKill(bool bShouldAutoKill, UQuickTweenable* instigator)
{
	ensureAlwaysMsgf(false, TEXT("SetAutoKill not implemented in IQuickTweenable"));
}

float UQuickTweenable::GetDuration() const
{
	ensureAlwaysMsgf(false, TEXT("GetDuration not implemented in IQuickTweenable"));
	return 0.0f;
}

float UQuickTweenable::GetElapsedTime() const
{
	ensureAlwaysMsgf(false, TEXT("GetElapsedTime not implemented in IQuickTweenable"));
	return 0.0f;
}

float UQuickTweenable::GetTimeScale() const
{
	ensureAlwaysMsgf(false, TEXT("GetTimeScale not implemented in IQuickTweenable"));
	return 0.0f;
}

bool UQuickTweenable::GetIsBackwards() const
{
	ensureAlwaysMsgf(false, TEXT("GetIsBackwards not implemented in IQuickTweenable"));
	return false;
}

bool UQuickTweenable::GetIsReversed() const
{
	ensureAlwaysMsgf(false, TEXT("GetIsReversed not implemented in IQuickTweenable"));
	return false;
}

EEaseType UQuickTweenable::GetEaseType() const
{
	ensureAlwaysMsgf(false, TEXT("GetEaseType not implemented in IQuickTweenable"));
	return static_cast<EEaseType>(0);
}

UCurveFloat* UQuickTweenable::GetEaseCurve() const
{
	ensureAlwaysMsgf(false, TEXT("GetEaseCurve not implemented in IQuickTweenable"));
	return nullptr;
}

int32 UQuickTweenable::GetLoops() const
{
	ensureAlwaysMsgf(false, TEXT("GetLoops not implemented in IQuickTweenable"));
	return 0;
}

ELoopType UQuickTweenable::GetLoopType() const
{
	ensureAlwaysMsgf(false, TEXT("GetLoopType not implemented in IQuickTweenable"));
	return static_cast<ELoopType>(0);
}

FString UQuickTweenable::GetTweenTag() const
{
	ensureAlwaysMsgf(false, TEXT("GetTweenTag not implemented in IQuickTweenable"));
	return FString();
}

int32 UQuickTweenable::GetCurrentLoop() const
{
	ensureAlwaysMsgf(false, TEXT("GetCurrentLoop not implemented in IQuickTweenable"));
	return 0;
}

bool UQuickTweenable::GetAutoKill() const
{
	ensureAlwaysMsgf(false, TEXT("GetAutoKill not implemented in IQuickTweenable"));
	return false;
}

bool UQuickTweenable::GetIsPendingKill() const
{
	ensureAlwaysMsgf(false, TEXT("GetIsPendingKill not implemented in IQuickTweenable"));
	return false;
}

bool UQuickTweenable::GetShouldPlayWhilePaused() const
{
	ensureAlwaysMsgf(false, TEXT("GetShouldPlayWhilePaused not implemented in IQuickTweenable"));
	return false;
}
