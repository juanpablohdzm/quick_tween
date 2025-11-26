// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/Latent/FQuickTweenLatentAction.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenLatentAction, Log, All);

FQuickTweenLatentAction::FQuickTweenLatentAction(
	const FLatentActionInfo& latentInfo,
	UQuickTweenSequence* tweenObj,
	EQuickTweenLatentSteps& outLatentStep)
	:	ExecutionFunction(latentInfo.ExecutionFunction),
		OutputLink(latentInfo.Linkage),
		CallbackTarget(latentInfo.CallbackTarget),
		bIsFirstUpdate(true),
		StepPtr(&outLatentStep)
{
	Buffer.Reserve(50);

	HandleStep(EQuickTweenLatentSteps::Default);

	tweenObj->OnStart.AddLambda([this](UQuickTweenSequence* Tween)
	{
		HandleStep(EQuickTweenLatentSteps::OnStart);
	});

	tweenObj->OnUpdate.AddLambda([this](UQuickTweenSequence* Tween)
	{
		HandleStep(EQuickTweenLatentSteps::OnUpdate);
	});

	tweenObj->OnComplete.AddLambda([this](UQuickTweenSequence* Tween)
	{
		HandleStep(EQuickTweenLatentSteps::OnComplete);
	});

	tweenObj->OnKilled.AddLambda([this](UQuickTweenSequence* Tween)
	{
		HandleStep(EQuickTweenLatentSteps::OnKilled);
	});
}

void FQuickTweenLatentAction::HandleStep(EQuickTweenLatentSteps NewStep)
{
	Buffer.Push(NewStep);
}

void FQuickTweenLatentAction::UpdateOperation(FLatentResponse& Response)
{
	if (!StepPtr || !CallbackTarget.IsValid())
	{
		Response.DoneIf(true);
		return;
	}

	if (Buffer.Num() > 0)
	{
		EQuickTweenLatentSteps step = Buffer[0];
		Buffer.RemoveAt(0);
		if (*StepPtr != step || step == EQuickTweenLatentSteps::OnUpdate || bIsFirstUpdate)
		{
			*StepPtr = step;
			Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
			if ((*StepPtr == EQuickTweenLatentSteps::OnComplete || *StepPtr == EQuickTweenLatentSteps::OnKilled) && Buffer.IsEmpty())
			{
				Response.DoneIf(true);
			}
		}
	}
	else if (*StepPtr == EQuickTweenLatentSteps::OnUpdate)
	{
		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
	}
	bIsFirstUpdate = false;
}
