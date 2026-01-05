// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Latent/FQuickTweenLatentAction.h"
#include "Tweens/QuickTweenBase.h"
#include "Utils/CommonValues.h"
#include "Utils/LoopType.h"
#include "QuickTweenLatentLibrary.generated.h"

class UQuickEmptyTween;
class USpringArmComponent;
class UCameraComponent;
class UImage;
class UWidget;
class UQuickIntTween;
class UQuickColorTween;
class UQuickVector2DTween;
class UQuickTweenable;
class UQuickTweenSequence;
class UQuickVectorTween;
class UQuickRotatorTween;
class UQuickFloatTween;
class UMaterialInstanceDynamic;

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
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false);

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
	 * @param setter Callback used to apply interpolated FVector values.
	 * @param duration Duration of the loop in seconds.
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
		const FVector& from,
		const FVector& to,
		FVectorSetter setter,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
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
	 * @param setter Callback used to apply interpolated FRotator values.
	 * @param bUseShortestPath When true, interpolation will use the shortest rotational path.
	 * @param duration Duration of the loop in seconds.
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
		const FRotator& from,
		const FRotator& to,
		FRotatorSetter setter,
		bool bUseShortestPath,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
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
	 * @param setter Callback used to apply interpolated float values.
	 * @param duration Duration of the loop in seconds.
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
		FFloatSetter setter,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent 2D vector tween.
	 *
	 * Tween an FVector2D value from \p from to \p to using the provided setter function.
	 * Exposed as a latent Blueprint node so execution can continue based on the latent step.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param from Starting 2D vector.
	 * @param to Target 2D vector.
	 * @param setter Callback used to apply interpolated FVector2D values.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickVector2DTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Vector | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickVector2DTween* QuickTweenCreateLatentTweenVector2D(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		const FVector2D& from,
		const FVector2D& to,
		FVector2DSetter setter,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent color tween.
	 *
	 * Tween an FColor value from \p from to \p to using the provided setter function.
	 * Exposed as a latent Blueprint node so execution can continue based on the latent step.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param from Starting color.
	 * @param to Target color.
	 * @param setter Callback used to apply interpolated FColor values.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickColorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Color | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickColorTween* QuickTweenCreateLatentTweenColor(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		const FColor& from,
		const FColor& to,
		FColorSetter setter,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent integer tween.
	 *
	 * Tween an integer value from \p from to \p to using the provided setter function.
	 * Exposed as a latent Blueprint node so execution can continue based on the latent step.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param from Starting integer value.
	 * @param to Target integer value.
	 * @param setter Callback used to apply interpolated int32 values.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickIntTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Int | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickIntTween* QuickTweenCreateLatentTweenInt(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		int32 from,
		int32 to,
		FIntSetter setter,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent "empty" tween (no value) that can be used as a timed latent node in Blueprints.
	 *
	 * This function exposes a latent Blueprint node that creates a UQuickEmptyTween which only tracks time
	 * and latent execution steps (useful for delays, sequencing or timing without changing any property).
	 *
	 * @param worldContextObject World context for latent action execution (Blueprint-provided).
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param duration Duration of the empty tween in seconds.
	 * @param tweenTag Optional tag to identify the created tween.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickEmptyTween instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Empty | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickEmptyTween* QuickTweenCreateLatentTweenEmpty(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		float duration = 1.0f,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
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
		const FVector& to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		EQuickTweenSpace space = EQuickTweenSpace::WorldSpace,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent MoveTo tween for a UWidget.
	 *
	 * Moves the provided widget to the target position over the given duration.
	 * Exposed as a latent Blueprint node; execution can continue based on the
	 * expanded \c EQuickTweenLatentSteps output pins.
	 *
	 * @param worldContextObject World context used for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param widget Target widget to move.
	 * @param to Target local position for the widget.
	 * @param duration Duration of the move in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve; when provided it overrides \c easeType.
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag to identify the created tween.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickVector2DTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Vector | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickVector2DTween* QuickTweenLatentMoveTo_Widget(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UWidget* widget,
		const FVector2D& to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent MoveBy tween for a scene component.
	 *
	 * Moves the provided scene component by the specified delta vector over time.
	 * Exposed as a latent Blueprint node; execution can continue based on the
	 * expanded \c EQuickTweenLatentSteps output pins.
	 *
	 * @param worldContextObject World context used for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param component Scene component to move.
	 * @param by Delta vector to add to the component's current location.
	 * @param duration Duration of the move in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve; overrides \c easeType when provided.
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param space Whether movement uses world or local space.
	 * @param tweenTag Optional tag to identify the created tween.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickVectorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Vector | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenLatentMoveBy_SceneComponent(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		USceneComponent* component,
		const FVector& by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		EQuickTweenSpace space = EQuickTweenSpace::WorldSpace,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent MoveBy tween for a UWidget.
	 *
	 * Moves the provided widget by the specified delta over the given duration.
	 * Exposed as a latent Blueprint node; execution can continue based on the
	 * expanded \c EQuickTweenLatentSteps output pins.
	 *
	 * @param worldContextObject World context used for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param widget Target widget to move.
	 * @param by Delta 2D vector to add to the widget's current position.
	 * @param duration Duration of the move in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve; overrides \c easeType when provided.
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag to identify the created tween.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickVector2DTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Vector2D | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickVector2DTween* QuickTweenLatentMoveBy_Widget(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UWidget* widget,
		const FVector2D& by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
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
	 * @param duration Duration of the loop in seconds.
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
		const FVector& to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		EQuickTweenSpace space = EQuickTweenSpace::LocalSpace,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent ScaleTo tween for a UWidget.
	 *
	 * Tweens the widget's render scale to the specified target over time.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param widget Target widget whose scale will be changed.
	 * @param to Target 2D scale.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve.
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickVector2DTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Vector2D | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickVector2DTween* QuickTweenLatentScaleTo_Widget(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UWidget* widget,
		const FVector2D& to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent ScaleBy tween for a scene component.
	 *
	 * Tweens the scale of the provided scene component by the given delta.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param component Scene component whose scale will be adjusted.
	 * @param by Delta scale to be applied to current scale.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve.
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param space Whether the delta applies in WorldSpace or LocalSpace.
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickVectorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Vecto | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenLatentScaleBy_SceneComponent(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		USceneComponent* component,
		const FVector& by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		EQuickTweenSpace space = EQuickTweenSpace::LocalSpace,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent ScaleBy tween for a UWidget.
	 *
	 * Tweens the widget's render scale by the specified delta.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param widget Target widget whose scale will be changed.
	 * @param by 2D delta scale to apply to current scale.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve.
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickVector2DTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Vector2D | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickVector2DTween* QuickTweenLatentScaleBy_Widget(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UWidget* widget,
		const FVector2D& by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
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
	 * @param to Target rotator (should be in target space).
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
        const FRotator& to,
        bool bUseShortestPath = true,
        float duration = 1.0f,
        float timeScale = 1.0f,
        EEaseType easeType = EEaseType::Linear,
        UCurveFloat* easeCurve = nullptr,
        int32 loops = 1,
        ELoopType loopType = ELoopType::Restart,
        EQuickTweenSpace space = EQuickTweenSpace::LocalSpace,
        const FString& tweenTag = "",
        bool bShouldAutoKill = true,
        bool bShouldPlayWhilePaused = false,
        bool bShouldAutoPlay = false);

	/**
	 * Create a latent RotateTo tween for a widget.
	 *
	 * Tweens a widget's rotation value to the target angle over the specified duration.
	 * This is exposed as a latent Blueprint node so execution can continue based on
	 * the latent step enum (EQuickTweenLatentSteps).
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param widget Target widget whose rotation will be changed.
	 * @param to Target rotation value (in degrees).
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickFloatTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Flo | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenLatentRotateTo_Widget(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UWidget* widget,
		float to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
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
		const FRotator& by,
		bool bUseShortestPath = true,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		EQuickTweenSpace space = EQuickTweenSpace::LocalSpace,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent RotateBy tween for a widget.
	 *
	 * Rotates the provided widget by the specified delta over time. Exposed as a latent
	 * Blueprint node so execution can continue based on the latent step.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param widget Widget to rotate.
	 * @param by Rotation delta to apply (added to current rotation).
	 * @param duration Duration of the rotation in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickRotatorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Rotator | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenLatentRotateBy_Widget(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UWidget* widget,
		float by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
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
        const FVector& to,
        bool bUseShortestPath = true,
        float duration = 1.0f,
        float timeScale = 1.0f,
        EEaseType easeType = EEaseType::Linear,
        UCurveFloat* easeCurve = nullptr,
        int32 loops = 1,
        ELoopType loopType = ELoopType::Restart,
        const FString& tweenTag = "",
        bool bShouldAutoKill = true,
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
        const FVector& point,
        const FVector& normal,
        float duration = 1.0f,
        float timeScale = 1.0f,
        EEaseType easeType = EEaseType::Linear,
        UCurveFloat* easeCurve = nullptr,
        int32 loops = 1,
        ELoopType loopType = ELoopType::Restart,
        const FString& tweenTag = "",
        bool bShouldAutoKill = true,
        bool bShouldPlayWhilePaused = false,
        bool bShouldAutoPlay = false);

	/**
	 * Create a latent ChangeColor tween for a UImage widget.
	 *
	 * Tweens the image's color to the specified target color over time.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param widget UImage widget whose color will be changed.
	 * @param to Target color.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickColorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Color | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickColorTween* QuickTweenLatentChangeColorTo_Image(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UImage* widget,
		const FColor& to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent ChangeOpacity tween for a generic widget.
	 *
	 * Tweens a widget's opacity to the given value over the specified duration.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param widget Target widget whose opacity will be changed.
	 * @param to Target opacity value (0.0 - 1.0).
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickFloatTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Float | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenLatentChangeOpacityTo_Widget(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UWidget* widget,
		float to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent ChangeFov tween for a camera component.
	 *
	 * Tweens the camera's field of view to the target value over time.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param camera Camera component whose FOV will be changed.
	 * @param to Target FOV value in degrees.
	 * @param duration Duration of the loop in seconds.
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
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Float | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenLatentChangeFovTo_Camera(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UCameraComponent* camera,
		float to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent ChangeDistance tween for a spring arm (to target).
	 *
	 * Tweens the spring arm's target arm length to the specified value.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param springArm Spring arm component whose length will be changed.
	 * @param to Target arm length.
	 * @param duration Duration of the loop in seconds.
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
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Float | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenLatentChangeDistanceTo_SpringArm(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		USpringArmComponent* springArm,
		float to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent ChangeDistanceBy tween for a spring arm (by delta).
	 *
	 * Tweens the spring arm's target arm length by adding the given delta to the current length.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param springArm Spring arm component whose length will be changed.
	 * @param by Delta to add to the current arm length.
	 * @param duration Duration of the loop in seconds.
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
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Float | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenLatentChangeDistanceBy_SpringArm(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		USpringArmComponent* springArm,
		float by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent VectorParameterTo tween for a material instance.
	 *
	 * Tweens the named vector parameter on the dynamic material to the target vector value.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param material Dynamic material instance to modify.
	 * @param parameterName Name of the vector parameter.
	 * @param to Target vector value.
	 * @param duration Duration of the loop in seconds.
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
	static UQuickVectorTween* QuickTweenLatentVectorParameterTo_Material(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UMaterialInstanceDynamic* material,
		const FName& parameterName,
		const FVector& to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent VectorParameterBy tween for a material instance.
	 *
	 * Tweens the named vector parameter on the dynamic material by the provided delta.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param material Dynamic material instance to modify.
	 * @param parameterName Name of the vector parameter.
	 * @param by Vector delta to add to the current parameter value.
	 * @param duration Duration of the loop in seconds.
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
	static UQuickVectorTween* QuickTweenLatentVectorParameterBy_Material(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UMaterialInstanceDynamic* material,
		const FName& parameterName,
		const FVector& by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent ScalarParameterTo tween for a material instance.
	 *
	 * Tweens the named scalar parameter on the dynamic material to the target value.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param material Dynamic material instance to modify.
	 * @param parameterName Name of the scalar parameter.
	 * @param to Target scalar value.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickFloatTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Float | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenLatentScalarParameterTo_Material(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UMaterialInstanceDynamic* material,
		const FName& parameterName,
		float to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent ScalarParameterBy tween for a material instance.
	 *
	 * Tweens the named scalar parameter on the dynamic material by the specified delta.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param material Dynamic material instance to modify.
	 * @param parameterName Name of the scalar parameter.
	 * @param by Delta to add to the current scalar parameter.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickFloatTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Float | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenLatentScalarParameterBy_Material(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UMaterialInstanceDynamic* material,
		const FName& parameterName,
		float by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent ColorParameterTo tween for a material instance.
	 *
	 * Tweens the named color parameter on the dynamic material to the target color.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param material Dynamic material instance to modify.
	 * @param parameterName Name of the color parameter.
	 * @param to Target color value.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickColorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Color | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickColorTween* QuickTweenLatentColorParameterTo_Material(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UMaterialInstanceDynamic* material,
		const FName& parameterName,
		const FColor& to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a latent ColorParameterBy tween for a material instance.
	 *
	 * Tweens the named color parameter on the dynamic material by the specified scalar delta.
	 * Typically used to adjust color channels uniformly by a value; implementation may interpret
	 * the delta according to the material setup.
	 *
	 * @param worldContextObject World context for latent action execution.
	 * @param latentInfo Latent action execution info supplied by Blueprint.
	 * @param latentStep Enum reference expanded as exec pins to control flow from Blueprint.
	 * @param material Dynamic material instance to modify.
	 * @param parameterName Name of the color parameter.
	 * @param by FColor delta to apply.
	 * @param duration Duration of the loop in seconds.
	 * @param timeScale Global time scale multiplier for the tween.
	 * @param easeType Built-in easing type to apply.
	 * @param easeCurve Optional custom ease curve (overrides easeType when provided).
	 * @param loops Number of times the tween will loop.
	 * @param loopType Looping behavior (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag for identification.
	 * @param bShouldAutoKill If true the tween will be auto-killed when finished.
	 * @param bShouldPlayWhilePaused If true the tween will update while paused.
	 * @param bShouldAutoPlay If true the tween will start immediately after creation.
	 * @return Pointer to the created UQuickColorTween.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "worldContextObject", Latent, LatentInfo = "latentInfo", ExpandEnumAsExecs = "latentStep", Keywords = "Tween | Color | Create | Make | Latent", HidePin = "latentStep"), Category = "QuickTween")
	static UQuickColorTween* QuickTweenLatentColorParameterBy_Material(
		UObject* worldContextObject,
		FLatentActionInfo latentInfo,
		EQuickTweenLatentSteps& latentStep,
		UMaterialInstanceDynamic* material,
		const FName& parameterName,
		const FColor& by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);
};
