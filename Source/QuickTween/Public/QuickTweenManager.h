#pragma once
#include "CoreMinimal.h"
#include "QuickTweenManager.generated.h"

UCLASS()
class QUICKTWEEN_API UQuickTweenManager final : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:

	static UQuickTweenManager* Get(const UObject* worldContextObject);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	UWorld* GetTickableGameObjectWorld() const override;
	virtual void Tick(float deltaTime) override;
	virtual TStatId GetStatId() const override;

	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual bool IsTickable() const override { return true; }
	virtual bool IsTickableInEditor() const override { return false; }
	virtual bool IsTickableWhenPaused() const override { return true; }

	void AddTween(class UQuickTweenable* tween);
	void RemoveTween(class UQuickTweenable* tween);

private:

	UPROPERTY(Transient)
	TArray<UQuickTweenable*> QuickTweens = {};

	bool bIsInitialized = false;
};
