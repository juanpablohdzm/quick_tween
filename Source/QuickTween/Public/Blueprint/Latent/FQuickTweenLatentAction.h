// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickTweenable.h"
#include "Tweens/QuickTweenBase.h"
#include "Tweens/QuickTweenSequence.h"

struct FLatentActionInfo;

UENUM()
enum class EQuickTweenLatentSteps : uint8
{
	Default,
	OnStart,
	OnUpdate,
	OnComplete,
	OnKilled
};

/**
 * 
 */
class FQuickTweenLatentAction : public FPendingLatentAction
{
public:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	bool bIsDone = false;
	bool bShotOnce = false;

	EQuickTweenLatentSteps* StepPtr = nullptr;

	FQuickTweenLatentAction(const FLatentActionInfo& latentInfo, UQuickTweenBase* obj, EQuickTweenLatentSteps& outLatentStep)
	{
		ExecutionFunction = latentInfo.ExecutionFunction;
		OutputLink = latentInfo.Linkage;
		CallbackTarget = latentInfo.CallbackTarget;
		StepPtr = &outLatentStep;
		*StepPtr = EQuickTweenLatentSteps::Default;
		bShotOnce = true;

		obj->OnStart.AddLambda([this](UQuickTweenBase* Tween)
		{
			OnTweenStarted(Cast<UQuickTweenable>(Tween));
		});
		obj->OnUpdate.AddLambda([this](UQuickTweenBase* Tween)
		{
			OnTweenUpdated(Cast<UQuickTweenable>(Tween));
		});
		obj->OnComplete.AddLambda([this](UQuickTweenBase* Tween)
		{
			OnTweenCompleted(Cast<UQuickTweenable>(Tween));
		});
		obj->OnKilled.AddLambda([this](UQuickTweenBase* Tween)
		{
			OnTweenKilled(Cast<UQuickTweenable>(Tween));
		});
	}

	FQuickTweenLatentAction(const FLatentActionInfo& latentInfo, UQuickTweenSequence* obj, EQuickTweenLatentSteps& outLatentStep)
	{
		ExecutionFunction = latentInfo.ExecutionFunction;
		OutputLink = latentInfo.Linkage;
		CallbackTarget = latentInfo.CallbackTarget;
		StepPtr = &outLatentStep;
		*StepPtr = EQuickTweenLatentSteps::Default;
		bShotOnce = true;

		obj->OnStart.AddLambda([this](UQuickTweenSequence* Tween)
		{
			OnTweenStarted(Cast<UQuickTweenable>(Tween));
		});
		obj->OnUpdate.AddLambda([this](UQuickTweenSequence* Tween)
		{
			OnTweenUpdated(Cast<UQuickTweenable>(Tween));
		});
		obj->OnComplete.AddLambda([this](UQuickTweenSequence* Tween)
		{
			OnTweenCompleted(Cast<UQuickTweenable>(Tween));
		});
		obj->OnKilled.AddLambda([this](UQuickTweenSequence* Tween)
		{
			OnTweenKilled(Cast<UQuickTweenable>(Tween));
		});
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		switch (*StepPtr)
		{
		case EQuickTweenLatentSteps::Default:
			if (bShotOnce)
			{
				Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
				bShotOnce = false;
			}
			break;
		case EQuickTweenLatentSteps::OnStart:
			if (bShotOnce)
			{
				Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
				bShotOnce = false;
			}
			break;
		case EQuickTweenLatentSteps::OnUpdate:
			Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
			break;
		case EQuickTweenLatentSteps::OnComplete:
			if (bShotOnce)
			{
				Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
				bShotOnce = false;
			}
			break;
		case EQuickTweenLatentSteps::OnKilled:
			if (bShotOnce)
			{
				Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
				bShotOnce = false;
			}
			break;
		default: ;
		}
		Response.FinishAndTriggerIf(bIsDone, ExecutionFunction, OutputLink, CallbackTarget);
	}

private:
	void OnTweenStarted(UQuickTweenable* Tween)
	{
		if (StepPtr)
		{
			*StepPtr = EQuickTweenLatentSteps::OnStart;
		}
		bShotOnce = true;
	}

	void OnTweenUpdated(UQuickTweenable* Tween)
	{
		if (StepPtr)
		{
			*StepPtr = EQuickTweenLatentSteps::OnUpdate;
		}
		bShotOnce = true;
	}

	void OnTweenCompleted(UQuickTweenable* Tween)
	{
		if (StepPtr)
		{
			*StepPtr = EQuickTweenLatentSteps::OnComplete;
		}
		bShotOnce = true;
		bIsDone = true;  // Mark latent action done to finish
	}

	void OnTweenKilled(UQuickTweenable* Tween)
	{
		if (StepPtr)
		{
			*StepPtr = EQuickTweenLatentSteps::OnKilled;
		}
		bShotOnce = true;
		bIsDone = true;  // Mark latent action done to finish
	}
};

