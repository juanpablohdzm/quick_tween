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
	UPROPERTY() int32 UpdateCalls  = 0;
	UPROPERTY() int32 CompleteCalls= 0;
	UPROPERTY() int32 KilledCalls  = 0;

	UFUNCTION() void HandleUpdate(UObject* Seq)  { ++UpdateCalls; }
	UFUNCTION() void HandleComplete(UObject* Seq){ ++CompleteCalls; }
	UFUNCTION() void HandleKilled(UObject* Seq)  { ++KilledCalls; }
};
