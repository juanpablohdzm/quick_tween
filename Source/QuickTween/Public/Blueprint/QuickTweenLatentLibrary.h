// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Latent/FQuickTweenLatentAction.h"
#include "Tweens/QuickTweenBase.h"
#include "Utils/CommonValues.h"
#include "Utils/LoopType.h"
#include "QuickTweenLatentLibrary.generated.h"

class UQuickTweenable;
class UQuickTweenSequence;
class UQuickVectorTween;
class UQuickRotatorTween;
class UQuickFloatTween;

/**
 * Blueprint-accessible library exposing latent QuickTween creation helpers.
 *
 * This class provides static UFUNCTIONs that create various types of tweens and sequences
 * as latent blueprint nodes. Each function accepts a latent info struct and an
 * ExpandEnumAsExecs parameter (EQuickTweenLatentSteps) so execution can continue
 * based on the tween step in Blueprint.
 */
UCLASS()
class QUICKTWEEN_API UQuickTweenLatentLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Create a latent QuickTween sequence.
	 *
	 * Creates and optionally starts a QuickTween sequence using latent execution in Blueprints.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param loops Number of times the sequence will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag to identify the created tween.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickTweenSequence.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Sequence | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickTweenSequence* QuickTweenCreateLatentSequence(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent vector tween.
	 *
	 * Tween a FVector value from \p from to \p to using the provided setter function.
	 * Exposed as a latent Blueprint node.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param from Starting vector.
	 * @param to Target vector.
	 * @param setterFunction Callback used to apply interpolated FVector values.
	 * @param duration Duration of the tween in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickVectorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Vector | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenCreateLatentTweenVector(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		FVector from,
		FVector to,
		FVectorSetter setterFunction,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);


	/**
	 * Create a latent rotator tween.
	 *
	 * Tween an FRotator value from \p from to \p to using the provided setter function.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param from Starting rotator.
	 * @param to Target rotator.
	 * @param setterFunction Callback used to apply interpolated FRotator values.
	 * @param bUseShortestPath When true, interpolation will use the shortest rotational path.
	 * @param duration Duration of the tween in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior.
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickRotatorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Rotator | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickRotatorTween* QuickTweenCreateLatentTweenRotator(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		FRotator from,
		FRotator to,
		FRotatorSetter setterFunction,
		bool bUseShortestPath,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent float tween.
	 *
	 * Tween a float value from \p from to \p to using the provided setter function.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param from Starting float value.
	 * @param to Target float value.
	 * @param setterFunction Callback used to apply interpolated float values.
	 * @param duration Duration of the tween in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior.
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickFloatTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Rotator | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenCreateLatentTweenFloat(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		float from,
		float to,
		FloatSetter setterFunction,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		FString tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);


	/**
	 * Create a latent MoveTo tween for a scene component.
	 *
	 * Moves the provided scene component to the target location over time.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param component Scene component to move.
	 * @param to Target world/local location depending on \p space.
	 * @param duration Duration of the move in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve.
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior.
	 * @param space Whether the target is in WorldSpace or LocalSpace.
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickVectorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Vector | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenLatentMoveTo_SceneComponent(
			UObject* worldContextObject,
			FLatentActionInfo latentInfo,
			EQuickTweenLatentSteps& latentStep,
			USceneComponent* component,
			FVector to,
			float duration = 1.0f,
			float timeScale = 1.0f,
			EEaseType easeType = EEaseType::Linear,
			UCurveFloat* easeCurve = nullptr,
			int32 loops = 1,
			ELoopType loopType = ELoopType::Restart,
			EQuickTweenSpace space = EQuickTweenSpace::WorldSpace,
			FString tweenTag = "",
			bool bShouldAutoKill = false,
			bool bShouldPlayWhilePaused = false,
			bool bShouldAutoPlay = false);

	/**
	 * Create a latent ScaleTo tween for a scene component.
	 *
	 * Tweens the scale of the provided scene component to \p to.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param component Scene component whose scale will be tweened.
	 * @param to Target scale.
	 * @param duration Duration of the tween in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve.
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior.
	 * @param space Whether the target is in WorldSpace or LocalSpace.
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickVectorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Vector | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenLatentScaleTo_SceneComponent(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		USceneComponent* component,
		FVector to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		EQuickTweenSpace space = EQuickTweenSpace::LocalSpace,
		FString tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent RotateTo tween for a scene component.
	 *
	 * Rotates the provided scene component to the target rotator.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param component Scene component to rotate.
	 * @param to Target rotator.
	 * @param bUseShortestPath When true, interpolation will use the shortest rotational path.
	 * @param duration Duration of the rotation in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve.
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior.
	 * @param space Whether the target is in WorldSpace or LocalSpace.
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickRotatorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Rotator | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickRotatorTween* QuickTweenLatentRotateTo_SceneComponent(
        	UObject* worldContextObject,
        	FLatentActionInfo latentInfo,
			EQuickTweenLatentSteps& latentStep,
        	USceneComponent* component,
        	FRotator to,
        	bool bUseShortestPath = true,
        	float duration = 1.0f,
        	float timeScale = 1.0f,
        	EEaseType easeType = EEaseType::Linear,
        	UCurveFloat* easeCurve = nullptr,
        	int32 loops = 1,
        	ELoopType loopType = ELoopType::Restart,
        	EQuickTweenSpace space = EQuickTweenSpace::LocalSpace,
        	FString tweenTag = "",
        	bool bShouldAutoKill = false,
        	bool bShouldPlayWhilePaused = false,
        	bool bShouldAutoPlay = false);

	/**
	 * Create a latent RotateBy tween for a scene component.
	 *
	 * Rotates the provided scene component by the specified rotator delta over time.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param component Scene component to rotate.
	 * @param by Rotation delta to apply (added to current rotation).
	 * @param bUseShortestPath When true, interpolation will use the shortest rotational path.
	 * @param duration Duration of the rotation in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param space Whether rotation is applied in WorldSpace or LocalSpace.
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickRotatorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Rotator | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickRotatorTween* QuickTweenLatentRotateBy_SceneComponent(
			UObject* worldContextObject,
			FLatentActionInfo latentInfo,
			EQuickTweenLatentSteps& latentStep,
			USceneComponent* component,
			FRotator by,
			bool bUseShortestPath = true,
			float duration = 1.0f,
			float timeScale = 1.0f,
			EEaseType easeType = EEaseType::Linear,
			UCurveFloat* easeCurve = nullptr,
			int32 loops = 1,
			ELoopType loopType = ELoopType::Restart,
			EQuickTweenSpace space = EQuickTweenSpace::LocalSpace,
			FString tweenTag = "",
			bool bShouldAutoKill = false,
			bool bShouldPlayWhilePaused = false,
			bool bShouldAutoPlay = false);

	/**
	 * Create a latent LookAt rotator tween for a scene component.
	 *
	 * Rotates the component to look at the specified target location.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param component Scene component to rotate.
	 * @param to Target location to look at.
	 * @param bUseShortestPath When true, interpolation will use the shortest rotational path.
	 * @param duration Duration of the rotation in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve.
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior.
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickRotatorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Rotator | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
    static UQuickRotatorTween* QuickTweenLatentLookAt_SceneComponent(
        	UObject* worldContextObject,
        	FLatentActionInfo latentInfo,
			EQuickTweenLatentSteps& latentStep,
        	USceneComponent* component,
        	FVector to,
        	bool bUseShortestPath = true,
        	float duration = 1.0f,
        	float timeScale = 1.0f,
        	EEaseType easeType = EEaseType::Linear,
        	UCurveFloat* easeCurve = nullptr,
        	int32 loops = 1,
        	ELoopType loopType = ELoopType::Restart,
        	FString tweenTag = "",
        	bool bShouldAutoKill = false,
        	bool bShouldPlayWhilePaused = false,
        	bool bShouldAutoPlay = false);

	/**
	 * Create a latent RotateAround tween for a scene component.
	 *
	 * Tweens a rotation value (float) around a point and axis (normal).
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param component Scene component that will be rotated around the point.
	 * @param from Starting angle in degrees.
	 * @param to Target angle in degrees.
	 * @param point Center point of rotation.
	 * @param normal Axis (normal) around which to rotate.
	 * @param duration Duration of the rotation in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve.
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior.
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickFloatTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Float | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
    static UQuickFloatTween* QuickTweenLatentRotateAroundPoint_SceneComponent(
        	UObject* worldContextObject,
        	FLatentActionInfo latentInfo,
			EQuickTweenLatentSteps& latentStep,
        	USceneComponent* component,
        	float from,
        	float to,
        	FVector point,
        	FVector normal,
        	float duration = 1.0f,
        	float timeScale = 1.0f,
        	EEaseType easeType = EEaseType::Linear,
        	UCurveFloat* easeCurve = nullptr,
        	int32 loops = 1,
        	ELoopType loopType = ELoopType::Restart,
        	FString tweenTag = "",
        	bool bShouldAutoKill = false,
        	bool bShouldPlayWhilePaused = false,
        	bool bShouldAutoPlay = false);
};