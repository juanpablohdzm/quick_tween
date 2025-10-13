#pragma once
#include "CoreMinimal.h"
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

	void AddTween(class IQuickTweenable* tween);
	void RemoveTween(class IQuickTweenable* tween);

private:
	FTSTicker::FDelegateHandle TickDelegateHandler;

	UPROPERTY()
	TArray<UObject*> QuickTweens = {};
};
