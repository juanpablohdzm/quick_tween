// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.


#include "Blueprint/Latent/FQuickTweenLatentAction.h"
#include "LatentActions.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenLatentAction, Log, All);

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
		const EQuickTweenLatentSteps nextStep = Buffer[0];
		Buffer.RemoveAt(0);

		const bool bIsNewStep = (*StepPtr != nextStep);
		const bool bShouldTriggerUpdate = (nextStep == EQuickTweenLatentSteps::OnUpdate);
		const bool bForceFirstTrigger = bIsFirstUpdate;

		// Trigger if it's new OR an OnUpdate OR forced by first update
		if (bIsNewStep || bShouldTriggerUpdate || bForceFirstTrigger)
		{
			*StepPtr = nextStep;
			Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
			if (nextStep == EQuickTweenLatentSteps::OnKilled)
			{
				Response.DoneIf(true);
				return;
			}
		}
	}
	else if (*StepPtr == EQuickTweenLatentSteps::OnUpdate)
	{
		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
	}

	bIsFirstUpdate = false;
}
