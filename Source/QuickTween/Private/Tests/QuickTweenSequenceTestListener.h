#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Tweens/QuickTweenSequence.h"
#include "QuickTweenSequenceTestListener.generated.h"

// A tiny UObject to bind to dynamic delegates in tests
UCLASS()
class UQuickTweenSequenceTestListener : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY() int32 StartCalls   = 0;
	UPROPERTY() int32 UpdateCalls  = 0;
	UPROPERTY() int32 CompleteCalls= 0;
	UPROPERTY() int32 KilledCalls  = 0;

	UFUNCTION() void HandleStart(UQuickTweenSequence* Seq)   { ++StartCalls; }
	UFUNCTION() void HandleUpdate(UQuickTweenSequence* Seq)  { ++UpdateCalls; }
	UFUNCTION() void HandleComplete(UQuickTweenSequence* Seq){ ++CompleteCalls; }
	UFUNCTION() void HandleKilled(UQuickTweenSequence* Seq)  { ++KilledCalls; }
};
