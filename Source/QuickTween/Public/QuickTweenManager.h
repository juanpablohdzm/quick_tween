#pragma once
#include "CoreMinimal.h"
#include "QuickTweenManager.generated.h"

UCLASS()
class QUICKTWEEN_API UQuickTweenManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:

	virtual void OnWorldBeginPlay(UWorld& inWorld) override;
	bool Tick(float deltaTime);
	~UQuickTweenManager() override;

private:
	FTSTicker::FDelegateHandle TickDelegateHandler;
};
