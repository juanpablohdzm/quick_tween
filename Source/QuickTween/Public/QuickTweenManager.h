// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QuickTweenManager.generated.h"

/**
 * World-scoped manager that updates active tweens each frame.
 * Implements a world subsystem to live with the UWorld and implements
 * FTickableGameObject so the engine will call Tick on this instance.
 *
 * Use UQuickTweenManager::Get(...) to retrieve the manager for a given
 * world context object.
 */
UCLASS()
class QUICKTWEEN_API UQuickTweenManager final : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:

	/**
	 * Retrieve the singleton instance of UQuickTweenManager for the given world context.
	 *
	 * @param worldContextObject - Any UObject that can be used to resolve the UWorld (e.g. Actor, Component, World).
	 * @return Pointer to the manager instance for the world, or nullptr if none is available.
	 */
	static UQuickTweenManager* Get(const UObject* worldContextObject);

	/**
	 * Initialize the subsystem. Called when the world subsystem is created.
	 *
	 * @param Collection - Subsystem collection info provided by the engine.
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * Deinitialize the subsystem. Called when the world subsystem is being destroyed.
	 * Use this to clean up any remaining tweens and resources.
	 */
	virtual void Deinitialize() override;


	/**
	 * Provide the world this tickable object belongs to.
	 *
	 * @return The UWorld pointer associated with this manager.
	 */
	virtual UWorld* GetTickableGameObjectWorld() const override;

	/**
	 * Per-frame tick function called by the engine.
	 *
	 * @param deltaTime - Time elapsed since the last tick (in seconds).
	 */
	virtual void Tick(float deltaTime) override;

	/**
	 * Return a unique stat id for profiling this tickable object.
	 */
	virtual TStatId GetStatId() const override;

	/**
	 * Always tick this object (even when not actively rendered).
	 */
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }

	/**
	 * Indicate whether this object should be ticked.
	 *
	 * @return true if tickable.
	 */
	virtual bool IsTickable() const override { return !QuickTweens.IsEmpty(); }

	/**
	 * Indicate whether this object is tickable in the editor.
	 *
	 * @return false to avoid editor ticks.
	 */
	virtual bool IsTickableInEditor() const override { return false; }

	/**
	 * Indicate whether this object should still tick when the game is paused.
	 *
	 * @return true to continue ticking while paused.
	 */
	virtual bool IsTickableWhenPaused() const override { return true; }

	/**
	 * Register a tween with the manager so it will be updated on Tick.
	 *
	 * @param tween - Pointer to the UQuickTweenable to add. Manager does not take ownership.
	 */
	void AddTween(class UQuickTweenable* tween);

	/**
	 * Unregister a tween so it will no longer be updated.
	 *
	 * @param tween - Pointer to the UQuickTweenable to remove.
	 */
	void RemoveTween(class UQuickTweenable* tween);

	/** Find a tween matching the given predicate.
	 *
	 * @param predicate - Function that returns true for the desired tween.
	 * @return A pointer to the found tween or null otherwise.
	 */
	UQuickTweenable* FindTweenByPredicate(TFunctionRef<bool(UQuickTweenable*)> predicate) const;

	/** Find all tweens matching the given predicate.
	 *
	 * @param predicate - Function that returns true for the desired tweens.
	 * @return An array of pointers to the found tweens.
	 */
	TArray<UQuickTweenable*> FindAllTweensByPredicate(TFunctionRef<bool(UQuickTweenable*)> predicate) const;
private:

	/** Array of active tweens managed by this subsystem. Transient so not serialized. */
	UPROPERTY(Transient)
	TArray<UQuickTweenable*> QuickTweens = {};

	/** Whether Initialize has been run for this manager instance. */
	bool bIsInitialized = false;
};
