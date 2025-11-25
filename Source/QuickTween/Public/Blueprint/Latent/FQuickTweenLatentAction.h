// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenable.h"
#include "Tweens/QuickTweenBase.h"
#include "Tweens/QuickTweenSequence.h"

struct FLatentActionInfo;

/**
 * 
 */
class FQuickTweenLatentAction : public FPendingLatentAction
{
public:
	FName StartFunction = TEXT("OnStart");
	FName UpdateFunction = TEXT("OnUpdate");
	FName CompleteFunction = TEXT("OnComplete");

	int32 StartLink = 1;
	int32 UpdateLink = 2;
	int32 CompleteLink = 3;

	FWeakObjectPtr CallbackTarget;
	bool bIsDone = false;

	FQuickTweenLatentAction(const FLatentActionInfo& LatentInfo, UQuickTweenBase* Obj)
	{
		CallbackTarget = LatentInfo.CallbackTarget;
		CompleteFunction = LatentInfo.ExecutionFunction;
		CompleteLink = LatentInfo.Linkage;

		Obj->OnStart.AddLambda([this](UQuickTweenBase* Tween)
		{
			OnTweenStarted(Cast<UQuickTweenable>(Tween));
		});
		Obj->OnUpdate.AddLambda([this](UQuickTweenBase* Tween)
		{
			OnTweenUpdated(Cast<UQuickTweenable>(Tween));
		});
		Obj->OnComplete.AddLambda([this](UQuickTweenBase* Tween)
		{
			OnTweenCompleted(Cast<UQuickTweenable>(Tween));
		});
		Obj->OnKilled.AddLambda([this](UQuickTweenBase* Tween)
		{
			OnTweenKilled(Cast<UQuickTweenable>(Tween));
		});
	}

	FQuickTweenLatentAction(const FLatentActionInfo& LatentInfo, UQuickTweenSequence* Obj)
	{
		CallbackTarget = LatentInfo.CallbackTarget;
		CompleteFunction = LatentInfo.ExecutionFunction;
		CompleteLink = LatentInfo.Linkage;

		Obj->OnStart.AddLambda([this](UQuickTweenSequence* Tween)
		{
			OnTweenStarted(Cast<UQuickTweenable>(Tween));
		});
		Obj->OnUpdate.AddLambda([this](UQuickTweenSequence* Tween)
		{
			OnTweenUpdated(Cast<UQuickTweenable>(Tween));
		});
		Obj->OnComplete.AddLambda([this](UQuickTweenSequence* Tween)
		{
			OnTweenCompleted(Cast<UQuickTweenable>(Tween));
		});
		Obj->OnKilled.AddLambda([this](UQuickTweenSequence* Tween)
		{
			OnTweenKilled(Cast<UQuickTweenable>(Tween));
		});
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		// Finish latent action when bIsDone is true
		Response.FinishAndTriggerIf(bIsDone, CompleteFunction, CompleteLink, CallbackTarget);
	}

	void Trigger(FName Func, int32 Link)
	{
		if (UObject* Obj = CallbackTarget.Get())
		{
			FLatentResponse LatentResponse{0.01f};
			LatentResponse.TriggerLink(Func, Link, Obj);
		}
	}

private:
	void OnTweenStarted(UQuickTweenable* Tween)
	{
		Trigger(StartFunction, StartLink);
	}

	void OnTweenUpdated(UQuickTweenable* Tween)
	{
		Trigger(UpdateFunction, UpdateLink);
	}

	void OnTweenCompleted(UQuickTweenable* Tween)
	{
		bIsDone = true;  // Mark latent action done to finish
		Trigger(CompleteFunction, CompleteLink);
	}

	void OnTweenKilled(UQuickTweenable* Tween)
	{
		bIsDone = true;  // Mark latent action done to finish
	}
};

