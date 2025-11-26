// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/QuickTweenLatentLibrary.h"

#include "Tweens/QuickRotatorTween.h"
#include "Components/SceneComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenLatentLibrary, Log, All);


UQuickTweenSequence* UQuickTweenLatentLibrary::QuickTweenCreateLatentSequence(
	UObject* worldContextObject,
	FLatentActionInfo latentInfo,
	EQuickTweenLatentSteps& latentStep,
	int32 loops,
	ELoopType loopType,
	const FString& tweenTag,
	bool bShouldAutoKill,
	bool bShouldPlayWhilePaused,
	bool bShouldAutoPlay)
{
	if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(worldContextObject))
    {
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

		if (LatentActionManager.FindExistingAction<FQuickTweenLatentAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr)
		{
			UQuickTweenSequence* sequence = NewObject<UQuickTweenSequence>();
			sequence->SetUp(
				worldContextObject,
				loops,
				loopType,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);

			LatentActionManager.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FQuickTweenLatentAction(latentInfo, sequence, latentStep));
			if (bShouldAutoPlay)
			{
				sequence->Play();
			}
			return sequence;
		}
    }
	return nullptr;
}
