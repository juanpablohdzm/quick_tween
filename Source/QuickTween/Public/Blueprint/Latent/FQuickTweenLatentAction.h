// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"
#include "Engine/LatentActionManager.h"
#include "FQuickTweenLatentAction.generated.h"

struct FLatentResponse;

/**
 * Enumeration used by the latent action to indicate the current step/state
 * of a QuickTween when reported back to Blueprint latent nodes.
 *
 * Each value represents a lifecycle point of a tween so Blueprint code
 * waiting on the latent action can branch or resume execution accordingly.
 */
UENUM()
enum class EQuickTweenLatentSteps : uint8
{
	/** Initial/default state before any tween events have been received. */
	Default,
	/** The tween has started (OnStart event). */
	OnStart,
	/** The tween has produced an update/tick (OnUpdate event). */
	OnUpdate,
	/** The tween has looped (OnLoop event). */
	OnLoop,
	/** The tween completed successfully (OnComplete event). */
	OnComplete,
	/** The tween will no longer be usable. */
	OnKilled
};

/**
 * Latent action used to bridge QuickTween events to Blueprint latent nodes.
 *
 * This class implements FPendingLatentAction so Blueprint nodes can wait on
 * tween lifecycle events (start, update, complete, killed). It buffers events
 * raised from the tween and drives the latent response in UpdateOperation.
 */
class FQuickTweenLatentAction : public FPendingLatentAction
{
public:
	/**
	 * Construct a latent action that listens to `tweenObj` events and reports steps
	 * into `outLatentStep`.
	 *
	 * @tparam T Type of the tween object (must expose OnStart/OnUpdate/OnComplete/OnKilled delegates).
	 * @param latentInfo Standard latent info provided by Blueprint node plumbing.
	 * @param tweenObj Pointer to the tween object to bind event handlers to.
	 * @param outLatentStep Reference to an enum value that will be updated with the current step.
	 */
	template<typename T>
	FQuickTweenLatentAction(const FLatentActionInfo& latentInfo, T* tweenObj, EQuickTweenLatentSteps& outLatentStep);

	/**
	 * FPendingLatentAction override called each tick by the latent system.
	 *
	 * This method consumes buffered tween steps and signals the latent node to
	 * continue execution when appropriate. The implementation must update the
	 * external `outLatentStep` via `StepPtr`.
	 */
	virtual void UpdateOperation(FLatentResponse& Response) override;

private:
	/** Name of the Blueprint function to resume when the latent action completes an iteration. */
	FName ExecutionFunction;

	/** Link index used by the latent system to determine where to resume execution. */
	int32 OutputLink;

	/** Weak pointer to the object that will receive the latent callback. */
	FWeakObjectPtr CallbackTarget;

	/** Tracks whether UpdateOperation has been called for the first time. */
	bool bIsFirstUpdate;

	/** Pointer to the external enum instance supplied by the caller to report the current step. */
	EQuickTweenLatentSteps* StepPtr = nullptr;

	/** FIFO buffer of steps received from the tween delegates to be processed on the game thread. */
	TArray<EQuickTweenLatentSteps> Buffer;

	/**
	 * Enqueue and process a new step reported by the tween.
	 *
	 * The method is safe to call from delegate lambdas and will ensure the
	 * step is observed during the next UpdateOperation.
	 *
	 * @param NewStep The newly reported lifecycle step.
	 */
	void HandleStep(EQuickTweenLatentSteps NewStep);
};

template <typename T>
FQuickTweenLatentAction::FQuickTweenLatentAction(
	const FLatentActionInfo& latentInfo,
	T* tweenObj,
	EQuickTweenLatentSteps& outLatentStep)
	:	ExecutionFunction(latentInfo.ExecutionFunction),
		OutputLink(latentInfo.Linkage),
		CallbackTarget(latentInfo.CallbackTarget),
		bIsFirstUpdate(true),
		StepPtr(&outLatentStep)
{
	Buffer.Reserve(50);

	HandleStep(EQuickTweenLatentSteps::Default);

	tweenObj->OnStart.AddLambda([this](T*)
	{
		HandleStep(EQuickTweenLatentSteps::OnStart);
	});

	tweenObj->OnUpdate.AddLambda([this](T*)
	{
		HandleStep(EQuickTweenLatentSteps::OnUpdate);
	});

	tweenObj->OnComplete.AddLambda([this](T*)
	{
		HandleStep(EQuickTweenLatentSteps::OnComplete);
	});

	tweenObj->OnKilled.AddLambda([this](T*)
	{
		HandleStep(EQuickTweenLatentSteps::OnKilled);
	});

	tweenObj->OnLoop.AddLambda([this](T*)
	{
		HandleStep(EQuickTweenLatentSteps::OnLoop);
	});
}

