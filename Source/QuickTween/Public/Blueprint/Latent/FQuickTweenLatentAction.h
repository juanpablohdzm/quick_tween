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
	bool bIsFirstUpdate;

	EQuickTweenLatentSteps* StepPtr = nullptr;

	FQuickTweenLatentAction(const FLatentActionInfo& latentInfo, UQuickTweenSequence* tweenObj, EQuickTweenLatentSteps& outLatentStep);

	virtual void UpdateOperation(FLatentResponse& Response) override;

private:

	TArray<EQuickTweenLatentSteps> Buffer;

	void HandleStep(EQuickTweenLatentSteps NewStep);
};

