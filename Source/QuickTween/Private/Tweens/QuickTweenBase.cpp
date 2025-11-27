// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#include "Tweens/QuickTweenBase.h"

#include "QuickTweenManager.h"
#include "Utils/CommonValues.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenBase, Log, All);

UQuickTweenBase::~UQuickTweenBase()
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

void UQuickTweenBase::SetUp(
	float duration,
	float timeScale,
	EEaseType easeType,
	UCurveFloat* easeCurve,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	const UObject* worldContextObject,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	Duration = duration;
	TimeScale = timeScale;
	EaseType = easeType;
	EaseCurve = easeCurve;
	Loops = loops;
	LoopType = loopType;
	TweenTag = tweenTag;
	bAutoKill = bShouldAutoKill;
	bPlayWhilePaused = bShouldPlayWhilePaused;
	WorldContextObject = worldContextObject;

	UQuickTweenManager* manager = UQuickTweenManager::Get(WorldContextObject);
	if (!manager)
	{
		UE_LOG(LogQuickTweenBase, Log, TEXT("Failed to get QuickTweenManager for QuickTweenBase. Tweens will not be updated."));
	}
	else
	{
		manager->AddTween(this);
	}

	if (bShouldAutoPlay)
	{
		Play();
	}
}

void UQuickTweenBase::Update_Restart(float deltaTime, UQuickTweenable* instigator)
{
	ElapsedTime = !bIsReversed ?  ElapsedTime + deltaTime * GetTimeScale() : ElapsedTime - deltaTime * GetTimeScale();


	bool shouldComplete = false;
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
			const int32 loop = ElapsedTime / GetDuration();
			CurrentLoop = loop + 1;
			shouldComplete = loop <= 0 && FMath::IsNearlyZero(mod);
		}
	}

	if (Loops != INFINITE_LOOPS && shouldComplete)
	{
		Complete(instigator);
	}
}

void UQuickTweenBase::Update_PingPong(float deltaTime, UQuickTweenable* instigator)
{
	/*
	 * !backwards & !reversed = to end
	 *  backwards & reversed = to end
	 * !backwards & reversed = to beginning
	 * !backwards & reversed = to beginning
	 *
	 * */
	const bool isGoingForward = bIsBackwards == bIsReversed;

	bool shouldCompleteLoop = false;
	if (isGoingForward)
	{
		ElapsedTime += deltaTime * GetTimeScale();
		if (ElapsedTime >= GetDuration())
		{
			shouldCompleteLoop = true;
			ElapsedTime = GetDuration() - (ElapsedTime - GetDuration());
		}
	}
	else
	{
		ElapsedTime -= deltaTime * GetTimeScale();
		if (ElapsedTime <= 0.0f)
		{
			shouldCompleteLoop = true;
			ElapsedTime = -ElapsedTime;
		}
	}

	if (shouldCompleteLoop)
	{
		if (Loops != INFINITE_LOOPS && (!bIsReversed && CurrentLoop >= Loops || bIsReversed && (CurrentLoop - 1) <= 0))
		{
			Complete(instigator);
			return;
		}

		bIsBackwards = !bIsBackwards;
		CurrentLoop = !bIsReversed ? CurrentLoop + 1 : CurrentLoop - 1;
	}
}

void UQuickTweenBase::Update(float deltaTime, UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator) || GetIsCompleted() || !GetIsPlaying()) return;

	if (!bHasStarted)
	{
		bHasStarted = true;
		if (OnStart.IsBound())
		{
			OnStart.Broadcast(this);
		}
	}

	if (FMath::IsNearlyZero(GetDuration()))
	{
		Complete(instigator);
		return;
	}

	switch (LoopType)
	{
	case ELoopType::Restart:
		Update_Restart(deltaTime, instigator);
		break;
	case ELoopType::PingPong:
		Update_PingPong(deltaTime, instigator);
		break;
	default:
		ensureAlwaysMsgf(false, TEXT("LoopType %s is not implemented in UQuickTweenBase::Update"), *UEnum::GetValueAsString(LoopType));
	}
}

void UQuickTweenBase::AssignOnStartEvent(FDynamicDelegateTween callback)
{
	OnStart.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenBase::AssignOnUpdateEvent(FDynamicDelegateTween callback)
{
	OnUpdate.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenBase::AssignOnCompleteEvent(FDynamicDelegateTween callback)
{
	OnComplete.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenBase::AssignOnKilledEvent(FDynamicDelegateTween callback)
{
	OnKilled.AddUFunction(callback.GetUObject(), callback.GetFunctionName());
}

void UQuickTweenBase::RemoveAllOnStartEvent(const UObject* object)
{
	OnStart.RemoveAll(object);
}

void UQuickTweenBase::RemoveAllOnUpdateEvent(const UObject* object)
{
	OnUpdate.RemoveAll(object);
}

void UQuickTweenBase::RemoveAllOnCompleteEvent(const UObject* object)
{
	OnComplete.RemoveAll(object);
}

void UQuickTweenBase::RemoveAllOnKilledEvent(const UObject* object)
{
	OnKilled.RemoveAll(object);
}

void UQuickTweenBase::SetAutoKill(bool bShouldAutoKill, UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bAutoKill = bShouldAutoKill;
	if (GetIsCompleted())
	{
		bIsPendingKill = bAutoKill;
	}
}

void UQuickTweenBase::Play(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bIsPlaying = true;
	return;
}

void UQuickTweenBase::Pause(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bIsPlaying = false;
	return;
}

void UQuickTweenBase::Stop(UQuickTweenable* instigator)
{

	if (!InstigatorIsOwner(instigator)) return;

	if (!bIsPlaying && !bIsCompleted)
	{
		return;
	}

	bIsPlaying   = false;
	bIsCompleted = true;
	CurrentLoop  = 1;
	Restart(instigator);

	return;
}

void UQuickTweenBase::Reverse(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bIsReversed = !bIsReversed;

	return;
}

void UQuickTweenBase::Restart(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	bIsCompleted = false;
	bIsPlaying   = true;
	bHasStarted = false;

	float duration = GetLoopType() == ELoopType::PingPong ? GetDuration() : GetDuration() * GetLoops();
	ElapsedTime = bIsReversed ? duration : 0.0f;
	CurrentLoop = bIsReversed ? GetLoops() : 1;
	return;
}

void UQuickTweenBase::Complete(UQuickTweenable* instigator, bool bSnapToEnd)
{
	if (!InstigatorIsOwner(instigator)) return;
	if (bIsCompleted) return;

	bIsPlaying   = false;
	bIsCompleted = true;

	ElapsedTime = Duration;

	if (OnComplete.IsBound())
	{
		OnComplete.Broadcast(this);
	}

	if (bAutoKill)
	{
		Kill(nullptr);
	}

	return;
}

void UQuickTweenBase::Reset(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;

	ElapsedTime = 0.0f;
	Duration    = 0.0f;
	TimeScale   = 1.0f;
	bIsPlaying  = false;
	bIsCompleted= false;
	bIsBackwards= false;
	bIsReversed = false;
	bHasStarted = false;
	EaseType    = EEaseType::Linear;
	EaseCurve   = nullptr;
	Loops       = 0;
	LoopType    = ELoopType::Restart;
	CurrentLoop = 1;
	return;
}

void UQuickTweenBase::Kill(UQuickTweenable* instigator)
{
	if (!InstigatorIsOwner(instigator)) return;
	if (bIsPendingKill) return;

	bIsPendingKill = true;
	bIsPlaying     = false;

	if (OnKilled.IsBound())
	{
		OnKilled.Broadcast(this);
	}
}
