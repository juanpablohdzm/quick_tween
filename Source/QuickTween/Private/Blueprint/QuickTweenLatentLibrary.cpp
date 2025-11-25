// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/QuickTweenLatentLibrary.h"

#include "Blueprint/Latent/FQuickTweenLatentAction.h"
#include "Tweens/QuickFloatTween.h"
#include "Tweens/QuickRotatorTween.h"
#include "Components/SceneComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuickTweenLatentLibrary, Log, All);


UQuickTweenSequence* UQuickTweenLatentLibrary::QuickTweenCreateLatentSequence(
	UObject* worldContextObject,
	FLatentActionInfo LatentInfo,
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
		if (LatentActionManager.FindExistingAction<FQuickTweenLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			UQuickTweenSequence* sequence = NewObject<UQuickTweenSequence>();
			sequence->SetUp(
				worldContextObject,
				loops,
				loopType,
				tweenTag,
				bShouldAutoKill,
				bShouldPlayWhilePaused);
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FQuickTweenLatentAction(LatentInfo, sequence));
			if (bShouldAutoPlay)
			{
				sequence->Play();
			}
			return sequence;
		}
    }
	return nullptr;
}
