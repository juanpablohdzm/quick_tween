#pragma once
#include "CoreMinimal.h"
#include "Utils/Badge.h"
#include "QuickTweenManager.generated.h"

UCLASS()
class QUICKTWEEN_API UQuickTweenManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:

	static UQuickTweenManager* Get(const UObject* worldContextObject);

	virtual void OnWorldBeginPlay(UWorld& inWorld) override;
	bool Tick(float deltaTime);
	~UQuickTweenManager() override;

	void AddTween(class IQuickTweenable* tween, Badge<class UQuickTweenBase> badge);
	void AddTween(class IQuickTweenable* tween, Badge<class UQuickTweenSequence> badge);
	void RemoveTween(class IQuickTweenable* tween, Badge<class UQuickTweenBase> badge);
	void RemoveTween(class IQuickTweenable* tween, Badge<class UQuickTweenSequence> badge);

private:
	FTSTicker::FDelegateHandle TickDelegateHandler;

	UPROPERTY()
	TArray<UObject*> QuickTweens = {};
};
