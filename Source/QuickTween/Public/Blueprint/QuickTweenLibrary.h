// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utils/EaseType.h"
#include "Utils/LoopType.h"
#include "Utils/CommonValues.h"
#include "QuickTweenLibrary.generated.h"

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
 * UQuickTweenLibrary
 *
 * Blueprint function library exposing helpers to create and manage QuickTween
 * objects (sequences and tweens) from Blueprints and C++.
 *
 * This class is a static container (derived from UBlueprintFunctionLibrary)
 * providing factory functions for creating vector, rotator and float tweens,
 * plus convenience functions for common SceneComponent operations (move, rotate,
 * scale, look-at and rotate-around). All functions operate on the game thread
 * and require a valid world context object when applicable.
 */
UCLASS()
class QUICKTWEEN_API UQuickTweenLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Create a new Quick Tween sequence.
	 *
	 * @param worldContextObject  Context object used to find the world for the sequence.
	 * @param loops               Number of times to loop the sequence. Use -1 for infinite.
	 * @param loopType            How the sequence loops (Restart, PingPong, etc.).
	 * @param tweenTag            Optional tag to identify the created sequence.
	 * @param bShouldAutoKill     If true the sequence will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the sequence will update while game is paused.
	 * @return                    A newly created UQuickTweenSequence pointer.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Sequence | Create | Make", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickTweenSequence* QuickTweenCreateSequence(
		UObject* worldContextObject,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false);

	/**
	 * Create a vector tween that interpolates between two FVector values using delegate getters
	 * and applies the interpolated value via a setter delegate each tick.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param from Delegate that returns the starting FVector value.
	 * @param to Delegate that returns the target FVector value.
	 * @param setter Delegate invoked each update with the current interpolated FVector.
	 * @param duration Time in seconds for the tween to complete. Defaults to 1.0f.
	 * @param timeScale Multiplier applied to the tween time. Defaults to 1.0f.
	 * @param easeType Predefined easing function to use for interpolation. Defaults to EEaseType::Linear.
	 * @param easeCurve Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops Number of times to loop the tween. Use -1 for infinite. Defaults to 1.
	 * @param loopType How the tween loops (Restart, PingPong, etc.). Defaults to ELoopType::Restart.
	 * @param tweenTag Optional tag to identify the created tween. Defaults to empty string.
	 * @param bShouldAutoKill If true the tween will be automatically killed when complete. Defaults to true.
	 * @param bShouldPlayWhilePaused If true the tween will update while game is paused. Defaults to false.
	 * @param bShouldAutoPlay If true the tween will start playing immediately after creation. Defaults to false.
	 * @return A pointer to the created UQuickVectorTween.
	 */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween | Vector", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenCreateTweenVector(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a rotator tween that interpolates between two rotator values using delegates.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param from Delegate that returns the starting rotation.
	 * @param to Delegate that returns the target rotation.
	 * @param setter Delegate invoked each update with the current interpolated rotator.
	 * @param bUseShortestPath If true, rotation will take the shortest angular path.
	 * @param duration Time in seconds for the tween to complete.
	 * @param timeScale Multiplier applied to the tween time.
	 * @param easeType Predefined easing function to use for interpolation.
	 * @param easeCurve Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops Number of times to loop the tween. Use -1 for infinite loops.
	 * @param loopType How the tween loops (e.g., Restart, PingPong).
	 * @param tweenTag Optional tag to identify the created tween.
	 * @param bShouldAutoKill If true, the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true, the tween will update while the game is paused.
	 * @param bShouldAutoPlay If true, the tween will start playing immediately after creation.
	 * @return A pointer to the created UQuickRotatorTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween | Rotator", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickRotatorTween* QuickTweenCreateTweenRotator(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a float tween that interpolates a value from `from` to `to` and applies it via `setter`.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param from Delegate that returns the starting float value.
	 * @param to Delegate that returns the target float value.
	 * @param setter Delegate invoked each update with the current interpolated float value.
	 * @param duration Time in seconds for the tween to complete.
	 * @param timeScale Multiplier applied to the tween time.
	 * @param easeType Predefined easing function to use for interpolation.
	 * @param easeCurve Optional custom `UCurveFloat` used for easing (overrides `easeType` when provided).
	 * @param loops Number of times to loop the tween. Use -1 for infinite loops.
	 * @param loopType How the tween loops (e.g., Restart, PingPong).
	 * @param tweenTag Optional tag to identify the created tween.
	 * @param bShouldAutoKill If true, the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true, the tween will update while the game is paused.
	 * @param bShouldAutoPlay If true, the tween will start playing immediately after creation.
	 * @return A pointer to the created `UQuickFloatTween`.
	 */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween | Float", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenCreateTweenFloat(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a Vector2D tween that interpolates between two FVector2D values and applies the
	 * interpolated value via the provided setter delegate each tick.
	 *
	 * The tween will evaluate easing using either a predefined EEaseType or an optional
	 * UCurveFloat. The start and target values are provided directly as parameters.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param from               Starting FVector2D value.
	 * @param to                 Target FVector2D value.
	 * @param setter     Delegate invoked each update with the current interpolated FVector2D.
	 * @param duration           Time in seconds for the tween to complete.
	 * @param timeScale          Multiplier applied to the tween time.
	 * @param easeType           Predefined easing function to use for interpolation.
	 * @param easeCurve          Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops              Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType           How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag           Optional tag to identify the created tween.
	 * @param bShouldAutoKill    If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay    If true the tween will start playing immediately after creation.
	 * @return                   Pointer to the created UQuickVector2DTween.
	 */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween | Vector", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVector2DTween* QuickTweenCreateTweenVector2D(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a Color tween that interpolates between two FColor values and applies the
	 * interpolated value via the provided setter delegate each tick.
	 *
	 * Easing can be controlled with a predefined EEaseType or by supplying a UCurveFloat.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param from               Starting FColor value.
	 * @param to                 Target FColor value.
	 * @param setter     Delegate invoked each update with the current interpolated FColor.
	 * @param duration           Time in seconds for the tween to complete.
	 * @param timeScale          Multiplier applied to the tween time.
	 * @param easeType           Predefined easing function to use for interpolation.
	 * @param easeCurve          Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops              Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType           How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag           Optional tag to identify the created tween.
	 * @param bShouldAutoKill    If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay    If true the tween will start playing immediately after creation.
	 * @return                   Pointer to the created UQuickColorTween.
	 */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween | Color", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickColorTween* QuickTweenCreateTweenColor(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create an Int tween that interpolates between two integer values and applies the
	 * interpolated value via the provided setter delegate each tick.
	 *
	 * The tween uses floating point easing internally but outputs integer values to the setter.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param from               Starting integer value.
	 * @param to                 Target integer value.
	 * @param setter     Delegate invoked each update with the current interpolated integer.
	 * @param duration           Time in seconds for the tween to complete.
	 * @param timeScale          Multiplier applied to the tween time.
	 * @param easeType           Predefined easing function to use for interpolation.
	 * @param easeCurve          Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops              Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType           How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag           Optional tag to identify the created tween.
	 * @param bShouldAutoKill    If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay    If true the tween will start playing immediately after creation.
	 * @return                   Pointer to the created UQuickIntTween.
	 */
	UFUNCTION(BlueprintPure, meta = (Keywords = "Tween | Int", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickIntTween* QuickTweenCreateTweenInt(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a vector tween that moves a SceneComponent to a target location.
	 *
	 * Note: The start value will be cached from the component's current location at the first update.
	 *
	 * @param worldContextObject  Context object used to find the world for the tween.
	 * @param component           The SceneComponent to move.
	 * @param to                  Target world-space location.
	 * @param duration            Time in seconds for the tween to complete.
	 * @param timeScale           Multiplier applied to the tween time.
	 * @param easeType            Predefined easing type to use.
	 * @param easeCurve           Optional custom curve to evaluate easing.
	 * @param loops               Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType            How the tween loops (Restart, PingPong, etc.).
	 * @param space          Space in which to perform the look-at operation (World or Local).
	 * @param tweenTag            Optional tag to identify the created tween.
	 * @param bShouldAutoKill     If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while game is paused.
	 * @param bShouldAutoPlay     If true the tween will start playing immediately after creation.
	 * @return                    A UQuickVectorTween pointer controlling the movement.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenMoveTo_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a Vector2D tween that moves a UWidget to a target absolute position.
	 *
	 * The widget's start position is sampled on the first update and the tween interpolates
	 * from that start position to the specified `to` value over `duration` seconds.
	 * Easing is controlled via `easeType` or an optional `easeCurve`. The returned tween
	 * will apply interpolated FVector2D values to the widget each tick.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param widget The UWidget to move.
	 * @param to Target position as an FVector2D.
	 * @param duration Time in seconds for the tween to complete (default 1.0f).
	 * @param timeScale Multiplier applied to the tween time (default 1.0f).
	 * @param easeType Predefined easing function to use (default EEaseType::Linear).
	 * @param easeCurve Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops Number of times to loop the tween (-1 for infinite, default 1).
	 * @param loopType How the tween loops (Restart, PingPong, etc., default ELoopType::Restart).
	 * @param tweenTag Optional tag to identify the created tween (default empty).
	 * @param bShouldAutoKill If true the tween will be automatically killed when complete (default false).
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused (default false).
	 * @param bShouldAutoPlay If true the tween will start playing immediately after creation (default false).
	 * @return Pointer to the created UQuickVector2DTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | Widget", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVector2DTween* QuickTweenMoveTo_Widget(
		UObject* worldContextObject,
		UWidget* widget,
		const FVector2D& to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);
	
	/**
	 * Create a vector tween that moves a SceneComponent by a relative offset.
	 *
	 * This function generates a UQuickVectorTween which will drive the component's world
	 * or local position from its current value (cached at the first update) by adding the
	 * provided `by` vector to the start position. The interpolated value is applied each
	 * tick via the tween's setter. Easing can be controlled with either a predefined
	 * EEaseType or an optional UCurveFloat. The operation can be performed in local or
	 * world space according to `space`.
	 *
	 * @param worldContextObject  Context object used to find the world for the tween.
	 * @param component           The SceneComponent to move.
	 * @param by                  Relative world/local offset to apply (added to the start location).
	 * @param duration            Time in seconds for the tween to complete.
	 * @param timeScale           Multiplier applied to the tween time.
	 * @param easeType            Predefined easing type to use for interpolation.
	 * @param easeCurve           Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops               Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType            How the tween loops (Restart, PingPong, etc.).
	 * @param space               Space in which to apply the movement (World or Local).
	 * @param tweenTag            Optional tag to identify the created tween.
	 * @param bShouldAutoKill     If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay     If true the tween will start playing immediately after creation.
	 * @return                    A UQuickVectorTween pointer controlling the movement.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenMoveBy_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a Vector2D tween that moves a UWidget by a relative offset.
	 *
	 * The tween samples the widget's current position/translation on its first update
	 * and interpolates from that start value to (start + by) over `duration` seconds.
	 * The interpolated FVector2D is applied to the widget each tick via the tween's
	 * internal setter. Easing may be controlled using a predefined `EEaseType` or by
	 * supplying a custom `UCurveFloat`.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param widget             The UWidget to move.
	 * @param by                 Relative offset to add to the widget's start position.
	 * @param duration           Time in seconds for the tween to complete.
	 * @param timeScale          Multiplier applied to the tween time.
	 * @param easeType           Predefined easing function to use for interpolation.
	 * @param easeCurve          Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops              Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType           How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag           Optional tag to identify the created tween.
	 * @param bShouldAutoKill    If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay    If true the tween will start playing immediately after creation.
	 * @return                   Pointer to the created UQuickVector2DTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | Widget", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVector2DTween* QuickTweenMoveBy_Widget(
		UObject* worldContextObject,
		UWidget* widget,
		const FVector2D& by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a vector tween that scales a SceneComponent to a target scale.
	 *
	 * Note: The start value will be cached from the component's current location at the first update.
	 *
	 * @param worldContextObject  Context object used to find the world for the tween.
	 * @param component           The SceneComponent to scale.
	 * @param to                  Target scale vector.
	 * @param duration            Time in seconds for the tween to complete.
	 * @param timeScale           Multiplier applied to the tween time.
	 * @param easeType            Predefined easing type to use.
	 * @param easeCurve           Optional custom curve to evaluate easing.
	 * @param loops               Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType            How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag            Optional tag to identify the created tween.
	 * @param space          Space in which to perform the look-at operation (World or Local).
	 * @param bShouldAutoKill     If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while game is paused.
	 * @param bShouldAutoPlay     If true the tween will start playing immediately after creation.
	 * @return                    A UQuickVectorTween pointer controlling the scale tween.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenScaleTo_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a Vector2D tween that scales a UWidget to a target absolute scale.
	 *
	 * This function creates a UQuickVector2DTween which will sample the widget's
	 * current scale on the first update and interpolate from that start value to
	 * the provided `to` target over `duration` seconds. Easing may be controlled
	 * using a predefined `EEaseType` or by supplying a custom `UCurveFloat`.
	 *
	 * @param worldContextObject    Context object used to find the world for the tween.
	 * @param widget                The UWidget to scale.
	 * @param to                    Target absolute scale as an FVector2D.
	 * @param duration              Time in seconds for the tween to complete. Defaults to 1.0f.
	 * @param timeScale             Multiplier applied to the tween time. Defaults to 1.0f.
	 * @param easeType              Predefined easing function to use for interpolation. Defaults to EEaseType::Linear.
	 * @param easeCurve             Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops                 Number of times to loop the tween. Use -1 for infinite. Defaults to 1.
	 * @param loopType              How the tween loops (Restart, PingPong, etc.). Defaults to ELoopType::Restart.
	 * @param tweenTag              Optional tag to identify the created tween. Defaults to empty string.
	 * @param bShouldAutoKill       If true the tween will be automatically killed when complete. Defaults to false.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused. Defaults to false.
	 * @param bShouldAutoPlay       If true the tween will start playing immediately after creation. Defaults to false.
	 * @return                      Pointer to the created UQuickVector2DTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | Widget", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVector2DTween* QuickTweenScaleTo_Widget(
		UObject* worldContextObject,
		UWidget* widget,
		const FVector2D& to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a vector tween that scales a SceneComponent by a relative amount.
	 *
	 * This function generates a UQuickVectorTween which will drive the component's scale
	 * from its current value (cached at the first update) by adding the provided \p by
	 * vector to the start scale. The scale change is applied every tick via the tween's
	 * setter. Easing can be controlled with either a predefined EEaseType or an optional
	 * UCurveFloat. The operation can be performed in local or world space according to \p space.
	 *
	 * @param worldContextObject    Context object used to find the world for the tween.
	 * @param component             The SceneComponent to scale.
	 * @param by                    Relative scale vector to apply (added to the start scale).
	 * @param duration              Time in seconds for the tween to complete.
	 * @param timeScale             Multiplier applied to the tween time.
	 * @param easeType              Predefined easing type to use for interpolation.
	 * @param easeCurve             Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops                 Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType              How the tween loops (Restart, PingPong, etc.).
	 * @param space                 Space in which to apply the scale (World or Local).
	 * @param tweenTag              Optional tag to identify the created tween.
	 * @param bShouldAutoKill       If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay       If true the tween will start playing immediately after creation.
	 * @return                      A UQuickVectorTween pointer controlling the scale tween.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenScaleBy_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a Vector2D tween that scales a UWidget by a relative amount.
	 *
	 * This function creates a UQuickVector2DTween which will drive the widget's scale
	 * from its current value (sampled at the first update) by adding the provided \p by
	 * vector to the start scale. Easing may be controlled using a predefined \p EEaseType
	 * or by supplying a custom \p UCurveFloat. The tween will apply the interpolated
	 * FVector2D value to the widget each tick.
	 *
	 * @param worldContextObject    Context object used to find the world for the tween.
	 * @param widget                The UWidget to scale.
	 * @param by                    Relative scale vector to apply (added to the start scale).
	 * @param duration              Time in seconds for the tween to complete.
	 * @param timeScale             Multiplier applied to the tween time.
	 * @param easeType              Predefined easing function to use for interpolation.
	 * @param easeCurve             Optional custom UCurveFloat used for easing (overrides \p easeType when provided).
	 * @param loops                 Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType              How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag              Optional tag to identify the created tween.
	 * @param bShouldAutoKill       If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay       If true the tween will start playing immediately after creation.
	 * @return                      Pointer to the created UQuickVector2DTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | Widget", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVector2DTween* QuickTweenScaleBy_Widget(
		UObject* worldContextObject,
		UWidget* widget,
		const FVector2D& by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a rotator tween that rotates a SceneComponent to a target rotator.
	 *
	 * Note: The start value will be cached from the component's current location at the first update.
	 *
	 * @param worldContextObject  Context object used to find the world for the tween.
	 * @param component           The SceneComponent to rotate.
	 * @param to                  Target rotation (should be in the target space).
	 * @param bUseShortestPath    If true rotation will take the shortest angular path.
	 * @param duration            Time in seconds for the tween to complete.
	 * @param timeScale           Multiplier applied to the tween time.
	 * @param easeType            Predefined easing type to use.
	 * @param easeCurve           Optional custom curve to evaluate easing.
	 * @param loops               Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType            How the tween loops (Restart, PingPong, etc.).
	 * @param space          Space in which to perform the look-at operation (World or Local).
	 * @param tweenTag            Optional tag to identify the created tween.
	 * @param bShouldAutoKill     If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while game is paused.
	 * @param bShouldAutoPlay     If true the tween will start playing immediately after creation.
	 * @return                    A UQuickRotatorTween pointer controlling the rotation.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickRotatorTween* QuickTweenRotateTo_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a float tween that rotates a UWidget to a target absolute angle.
	 *
	 * This tween drives a single float value representing the widget's rotation angle
	 * and interpolates from the widget's current rotation (sampled at first update)
	 * to the specified target \p to over \p duration seconds.
	 *
	 * Easing may be controlled using a predefined \p EEaseType or a custom \p UCurveFloat.
	 *
	 * @param worldContextObject    Context object used to find the world for the tween.
	 * @param widget                The UWidget to rotate.
	 * @param to                    Target absolute angle in degrees to rotate the widget to.
	 * @param duration              Time in seconds for the tween to complete.
	 * @param timeScale             Multiplier applied to the tween time.
	 * @param easeType              Predefined easing function to use for interpolation.
	 * @param easeCurve             Optional custom UCurveFloat used for easing (overrides \p easeType when provided).
	 * @param loops                 Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType              How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag              Optional tag to identify the created tween.
	 * @param bShouldAutoKill       If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay       If true the tween will start playing immediately after creation.
	 * @return                      Pointer to the created UQuickFloatTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | Widget", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenRotateTo_Widget(
		UObject* worldContextObject,
		UWidget* widget,
		float to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a rotator tween that rotates a SceneComponent by a relative rotator value.
	 *
	 * This method creates a `UQuickRotatorTween` which will drive the component's rotation
	 * from its current rotation (cached at the first update) by the provided `by` rotator.
	 * The rotation can be applied in world or local space according to `space`. When
	 * `bUseShortestPath` is true the interpolation will pick the shortest angular path
	 * for each axis.
	 *
	 * @param worldContextObject  Context object used to find the world for the tween.
	 * @param component           The SceneComponent to rotate.
	 * @param by                  Relative rotator to apply (added to the start rotation).
	 * @param bUseShortestPath    If true rotation will take the shortest angular path.
	 * @param duration            Time in seconds for the tween to complete.
	 * @param timeScale           Multiplier applied to the tween time.
	 * @param easeType            Predefined easing type to use.
	 * @param easeCurve           Optional custom curve to evaluate easing.
	 * @param loops               Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType            How the tween loops (Restart, PingPong, etc.).
	 * @param space               Space in which to apply the relative rotation (World or Local).
	 * @param tweenTag            Optional tag to identify the created tween.
	 * @param bShouldAutoKill     If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while game is paused.
	 * @param bShouldAutoPlay     If true the tween will start playing immediately after creation.
	 * @return                    A UQuickRotatorTween pointer controlling the rotation.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickRotatorTween* QuickTweenRotateBy_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a float tween that rotates a UWidget by a relative angle.
	 *
	 * The tween will drive a single float value representing the widget's rotation angle
	 * and apply a relative change of `by` degrees over `duration`. The widget's starting
	 * rotation is sampled when the tween first updates. Easing may be controlled using a
	 * predefined `EEaseType` or a custom `UCurveFloat`.
	 *
	 * @param worldContextObject    Context object used to find the world for the tween.
	 * @param widget                The UWidget to rotate.
	 * @param by                    Relative angle in degrees to rotate the widget (added to start rotation).
	 * @param duration              Time in seconds for the tween to complete.
	 * @param timeScale             Multiplier applied to the tween time.
	 * @param easeType              Predefined easing function to use for interpolation.
	 * @param easeCurve             Optional custom `UCurveFloat` used for easing (overrides `easeType` when provided).
	 * @param loops                 Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType              How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag              Optional tag to identify the created tween.
	 * @param bShouldAutoKill       If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay       If true the tween will start playing immediately after creation.
	 * @return                      Pointer to the created `UQuickFloatTween`.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | Widget", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenRotateBy_Widget(
		UObject* worldContextObject,
		UWidget* widget,
		float by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);



	/**
	 * Create a rotator tween that orients a SceneComponent to look at a target point.
	 *
	 * Note: The start value will be cached from the component's current location at the first update.
	 *
	 * @param worldContextObject  Context object used to find the world for the tween.
	 * @param component           The SceneComponent to rotate.
	 * @param to                  Target world-space location to look at.
	 * @param bUseShortestPath    If true rotation will take the shortest angular path.
	 * @param duration            Time in seconds for the tween to complete.
	 * @param timeScale           Multiplier applied to the tween time.
	 * @param easeType            Predefined easing type to use.
	 * @param easeCurve           Optional custom curve to evaluate easing.
	 * @param loops               Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType            How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag            Optional tag to identify the created tween.
	 * @param bShouldAutoKill     If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while game is paused.
	 * @param bShouldAutoPlay     If true the tween will start playing immediately after creation.
	 * @return                    A UQuickRotatorTween pointer controlling the look-at rotation.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickRotatorTween* QuickTweenLookAt_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);


	/**
	 * Create a float tween that rotates a SceneComponent around a specified point and axis.
	 *
	 * The tween will drive a single float value (angle) from \p from to \p to over \p duration
	 * and apply that rotation to \p component around \p point using \p normal as the rotation axis.
	 *
	 * Note: The starting position is determined by the current location of the component at the time of tween creation, unlike other functions that the starting position is determined at the first update.
	 *
	 * @param worldContextObject         Context object used to find the world for the tween.
	 * @param component                  The SceneComponent to rotate around the point.
	 * @param from                       Starting angle in degrees.
	 * @param to                         Target angle in degrees.
	 * @param point                      World-space point to rotate around.
	 * @param normal                     Axis (normal) to rotate around.
	 * @param duration                   Time in seconds for the tween to complete.
	 * @param timeScale                  Multiplier applied to the tween time.
	 * @param easeType                   Predefined easing type to use.
	 * @param easeCurve                  Optional custom curve to evaluate easing.
	 * @param loops                      Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType                   How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag                   Optional tag to identify the created tween.
	 * @param bShouldAutoKill            If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused     If true the tween will update while game is paused.
	 * @param bShouldAutoPlay            If true the tween will start playing immediately after creation.
	 * @return                           A UQuickFloatTween pointer controlling the rotation, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenRotateAroundPoint_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a color tween that animates a `UImage`'s color/tint to a target `FColor`.
	 *
	 * The widget's start color is sampled on the first update and the tween will
	 * interpolate from that start value to the specified `to` value over `duration`
	 * seconds. Easing can be controlled with a predefined `EEaseType` or by supplying
	 * a custom `UCurveFloat` (`easeCurve` overrides `easeType` when provided).
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param widget The `UImage` widget whose color will be animated.
	 * @param to Target color to animate to.
	 * @param duration Time in seconds for the tween to complete (default 1.0f).
	 * @param timeScale Multiplier applied to the tween time (default 1.0f).
	 * @param easeType Predefined easing function to use for interpolation.
	 * @param easeCurve Optional custom `UCurveFloat` used for easing (overrides `easeType` when provided).
	 * @param loops Number of times to loop the tween (-1 for infinite).
	 * @param loopType How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag to identify the created tween.
	 * @param bShouldAutoKill If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay If true the tween will start playing immediately after creation.
	 * @return Pointer to the created `UQuickColorTween`, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Color | Widget", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickColorTween* QuickTweenChangeColorTo_Image(
		UObject* worldContextObject,
		UImage* widget,
		const FColor& to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a float tween that animates a UWidget's opacity to a target value.
	 *
	 * The widget's start opacity is sampled on the first update and the tween will
	 * interpolate from that start value to the specified `to` value over `duration`
	 * seconds. Easing may be controlled using a predefined `EEaseType` or by
	 * supplying a custom `UCurveFloat` (`easeCurve` overrides `easeType` when set).
	 *
	 * @param worldContextObject    Context object used to find the world for the tween.
	 * @param widget                The UWidget whose opacity will be animated.
	 * @param to                    Target opacity value (0.0 = transparent, 1.0 = opaque).
	 * @param duration              Time in seconds for the tween to complete.
	 * @param timeScale             Multiplier applied to the tween time.
	 * @param easeType              Predefined easing function to use for interpolation.
	 * @param easeCurve             Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops                 Number of times to loop the tween (-1 for infinite).
	 * @param loopType              How the tween loops (e.g., Restart, PingPong).
	 * @param tweenTag              Optional tag to identify the created tween.
	 * @param bShouldAutoKill       If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay       If true the tween will start playing immediately after creation.
	 * @return                      Pointer to the created UQuickFloatTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Color | Widget", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenChangeOpacityTo_Widget(
		UObject* worldContextObject,
		UWidget* widget,
		float to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Animate camera field of view (FOV) to a target value.
	 *
	 * Creates a UQuickFloatTween that interpolates the camera's FOV from the
	 * camera's current FOV (sampled at first update) to the provided \p to value
	 * over \p duration seconds. Easing can be selected via \p easeType or an
	 * optional \p easeCurve (when provided, \p easeCurve overrides \p easeType).
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param camera             The UCameraComponent whose FOV will be animated.
	 * @param to                 Target FOV value in degrees.
	 * @param duration           Time in seconds for the tween to complete (default 1.0f).
	 * @param timeScale          Multiplier applied to the tween time (default 1.0f).
	 * @param easeType           Predefined easing function to use (default EEaseType::Linear).
	 * @param easeCurve          Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops              Number of times to loop the tween (-1 for infinite, default 1).
	 * @param loopType           How the tween loops (Restart, PingPong, etc., default ELoopType::Restart).
	 * @param tweenTag           Optional tag to identify the created tween (default empty).
	 * @param bShouldAutoKill    If true the tween will be automatically killed when complete (default false).
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused (default false).
	 * @param bShouldAutoPlay    If true the tween will start playing immediately after creation (default false).
	 * @return                   Pointer to the created UQuickFloatTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Color | Camera", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenChangeFovTo_Camera(
		UObject* worldContextObject,
		UCameraComponent* camera,
		float to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Animate the spring arm's distance (TargetArmLength) to a target value.
	 *
	 * Creates a UQuickFloatTween that interpolates the spring arm's current arm length
	 * (sampled at the first update) to the provided \p to value over \p duration seconds.
	 * Easing can be controlled with a predefined \p EEaseType or by supplying a custom
	 * \p UCurveFloat (when provided, \p easeCurve overrides \p easeType).
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param springArm          The USpringArmComponent whose TargetArmLength will be animated.
	 * @param to                 Target arm length (world units).
	 * @param duration           Time in seconds for the tween to complete.
	 * @param timeScale          Multiplier applied to the tween time.
	 * @param easeType           Predefined easing function to use for interpolation.
	 * @param easeCurve          Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops              Number of times to loop the tween (-1 for infinite).
	 * @param loopType           How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag           Optional tag to identify the created tween.
	 * @param bShouldAutoKill    If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay    If true the tween will start playing immediately after creation.
	 * @return                   Pointer to the created UQuickFloatTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Distance | Camera", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenChangeDistanceTo_SpringArm(
		UObject* worldContextObject,
		USpringArmComponent* springArm,
		float to,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a float tween that animates the spring arm's TargetArmLength by a relative amount.
	 *
	 * This function creates a UQuickFloatTween which samples the spring arm's current
	 * TargetArmLength on the first update and interpolates to (start + by) over \p duration seconds.
	 * Easing is controlled by \p easeType or overridden by \p easeCurve when provided.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param springArm The USpringArmComponent whose TargetArmLength will be animated.
	 * @param by Relative change to apply to the spring arm's current TargetArmLength.
	 * @param duration Time in seconds for the tween to complete.
	 * @param timeScale Multiplier applied to the tween time.
	 * @param easeType Predefined easing function to use for interpolation.
	 * @param easeCurve Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops Number of times to loop the tween (-1 for infinite).
	 * @param loopType How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag to identify the created tween.
	 * @param bShouldAutoKill If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay If true the tween will start playing immediately after creation.
	 * @return Pointer to the created UQuickFloatTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Distance | Camera", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenChangeDistanceBy_SpringArm(
		UObject* worldContextObject,
		USpringArmComponent* springArm,
		float by,
		float duration = 1.0f,
		float timeScale = 1.0f,
		EEaseType easeType = EEaseType::Linear,
		UCurveFloat* easeCurve = nullptr,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
		 * Create a vector tween that animates a vector parameter on a dynamic material instance.
		 *
		 * The tween will sample the material parameter's current value on the first update and
		 * interpolate from that start value to the provided `to` value over `duration` seconds.
		 * Easing is controlled using `easeType` or overridden by `easeCurve` when provided.
		 *
		 * @param worldContextObject Context object used to find the world for the tween.
		 * @param material           The UMaterialInstanceDynamic containing the vector parameter.
		 * @param parameterName      The name of the vector parameter to animate.
		 * @param to                 Target FVector value for the material parameter.
		 * @param duration           Time in seconds for the tween to complete.
		 * @param timeScale          Multiplier applied to the tween time.
		 * @param easeType           Predefined easing function to use for interpolation.
		 * @param easeCurve          Optional custom UCurveFloat used for easing (overrides easeType when provided).
		 * @param loops              Number of times to loop the tween. Use -1 for infinite.
		 * @param loopType           How the tween loops (Restart, PingPong, etc.).
		 * @param tweenTag           Optional tag to identify the created tween.
		 * @param bShouldAutoKill    If true the tween will be automatically killed when complete.
		 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
		 * @param bShouldAutoPlay    If true the tween will start playing immediately after creation.
		 * @return                   Pointer to the created UQuickVectorTween, or nullptr on failure.
		 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Vector | Material", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenVectorParameterTo_Material(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a vector tween that animates a vector parameter on a dynamic material instance by a relative amount.
	 *
	 * The tween samples the material parameter's current value on its first update and interpolates from that start
	 * value to (start + by) over the specified duration. Easing is controlled by \p easeType or overridden by
	 * \p easeCurve when provided.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param material The UMaterialInstanceDynamic containing the vector parameter.
	 * @param parameterName The name of the vector parameter to animate.
	 * @param by Relative FVector value to add to the sampled start value.
	 * @param duration Time in seconds for the tween to complete.
	 * @param timeScale Multiplier applied to the tween time.
	 * @param easeType Predefined easing function to use for interpolation.
	 * @param easeCurve Optional custom UCurveFloat used for easing (overrides \p easeType when provided).
	 * @param loops Number of times to loop the tween. Use -1 for infinite.
	 * @param loopType How the tween loops (Restart, PingPong, etc.).
	 * @param tweenTag Optional tag to identify the created tween.
	 * @param bShouldAutoKill If true the tween will be automatically killed when complete.
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused.
	 * @param bShouldAutoPlay If true the tween will start playing immediately after creation.
	 * @return Pointer to the created UQuickVectorTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Vector | Material", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickVectorTween* QuickTweenVectorParameterBy_Material(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);


	/**
	 * Create a float tween that animates a scalar parameter on a dynamic material instance.
	 *
	 * The tween will sample the material parameter's current value on the first update and
	 * interpolate from that start value to the provided `to` value over `duration` seconds.
	 * Easing is controlled using `easeType` or overridden by `easeCurve` when provided.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param material The UMaterialInstanceDynamic containing the scalar parameter.
	 * @param parameterName The name of the scalar parameter to animate.
	 * @param to Target float value for the material parameter.
	 * @param duration Time in seconds for the tween to complete (default 1.0f).
	 * @param timeScale Multiplier applied to the tween time (default 1.0f).
	 * @param easeType Predefined easing function to use for interpolation.
	 * @param easeCurve Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops Number of times to loop the tween (-1 for infinite, default 1).
	 * @param loopType How the tween loops (Restart, PingPong, etc., default ELoopType::Restart).
	 * @param tweenTag Optional tag to identify the created tween (default empty).
	 * @param bShouldAutoKill If true the tween will be automatically killed when complete (default false).
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused (default false).
	 * @param bShouldAutoPlay If true the tween will start playing immediately after creation (default false).
	 * @return Pointer to the created UQuickFloatTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Float | Material", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenScalarParameterTo_Material(
	UObject* worldContextObject,
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
	bool bShouldAutoKill = false,
	bool bShouldPlayWhilePaused = false,
	bool bShouldAutoPlay = false);


	/**
	 * Create a float tween that animates a scalar parameter on a dynamic material instance by a relative amount.
	 *
	 * The tween samples the material parameter's current value on the first update and interpolates
	 * from that start value to (start + by) over the specified duration. Easing is controlled by
	 * `easeType` or overridden by `easeCurve` when provided.
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param material The UMaterialInstanceDynamic containing the scalar parameter.
	 * @param parameterName The name of the scalar parameter to animate.
	 * @param by Relative float value to add to the sampled start value.
	 * @param duration Time in seconds for the tween to complete (default 1.0f).
	 * @param timeScale Multiplier applied to the tween time (default 1.0f).
	 * @param easeType Predefined easing function to use for interpolation.
	 * @param easeCurve Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops Number of times to loop the tween (-1 for infinite, default 1).
	 * @param loopType How the tween loops (Restart, PingPong, etc., default ELoopType::Restart).
	 * @param tweenTag Optional tag to identify the created tween (default empty).
	 * @param bShouldAutoKill If true the tween will be automatically killed when complete (default false).
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused (default false).
	 * @param bShouldAutoPlay If true the tween will start playing immediately after creation (default false).
	 * @return Pointer to the created UQuickFloatTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Float | Material", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickFloatTween* QuickTweenScalarParameterBy_Material(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a color tween that animates a vector/color parameter on a dynamic material instance to an absolute color.
	 *
	 * The material parameter's starting color is sampled on the first update and the tween will interpolate
	 * from that start value to the provided `to` FColor over `duration` seconds. Easing may be controlled
	 * using a predefined `EEaseType` or an optional `UCurveFloat` (`easeCurve` overrides `easeType` when provided).
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param material           The UMaterialInstanceDynamic containing the color parameter.
	 * @param parameterName      The name of the color parameter to animate.
	 * @param to                  Target FColor value for the material parameter.
	 * @param duration           Time in seconds for the tween to complete (default 1.0f).
	 * @param timeScale          Multiplier applied to the tween time (default 1.0f).
	 * @param easeType           Predefined easing function to use for interpolation (default EEaseType::Linear).
	 * @param easeCurve          Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops              Number of times to loop the tween (-1 for infinite, default 1).
	 * @param loopType           How the tween loops (Restart, PingPong, etc., default ELoopType::Restart).
	 * @param tweenTag           Optional tag to identify the created tween (default empty).
	 * @param bShouldAutoKill    If true the tween will be automatically killed when complete (default false).
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused (default false).
	 * @param bShouldAutoPlay    If true the tween will start playing immediately after creation (default false).
	 * @return                   Pointer to the created UQuickColorTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Color | Material", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickColorTween* QuickTweenColorParameterTo_Material(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Create a color tween that animates a material color parameter by a relative amount.
	 *
	 * The material parameter's starting value is sampled on the first update and the tween will interpolate
	 * from that start value to (start + by) over `duration` seconds. Note: the `by` parameter is provided
	 * as a float and should be interpreted according to how the material parameter consumes the value
	 * (e.g., intensity or a scalar channel modifier).
	 *
	 * @param worldContextObject Context object used to find the world for the tween.
	 * @param material           The UMaterialInstanceDynamic containing the color parameter.
	 * @param parameterName      The name of the color parameter to animate.
	 * @param by                 Relative FColor value to add to the sampled start value.
	 * @param duration           Time in seconds for the tween to complete (default 1.0f).
	 * @param timeScale          Multiplier applied to the tween time (default 1.0f).
	 * @param easeType           Predefined easing function to use for interpolation (default EEaseType::Linear).
	 * @param easeCurve          Optional custom UCurveFloat used for easing (overrides easeType when provided).
	 * @param loops              Number of times to loop the tween (-1 for infinite, default 1).
	 * @param loopType           How the tween loops (Restart, PingPong, etc., default ELoopType::Restart).
	 * @param tweenTag           Optional tag to identify the created tween (default empty).
	 * @param bShouldAutoKill    If true the tween will be automatically killed when complete (default false).
	 * @param bShouldPlayWhilePaused If true the tween will update while the game is paused (default false).
	 * @param bShouldAutoPlay    If true the tween will start playing immediately after creation (default false).
	 * @return                   Pointer to the created UQuickColorTween, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Tween | Color | Material", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickColorTween* QuickTweenColorParameterBy_Material(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = false,
		bool bShouldPlayWhilePaused = false,
		bool bShouldAutoPlay = false);

	/**
	 * Find an active QuickTween by its tag within the world context.
	 *
	 * @param worldContextObject  Context object used to locate the world that contains the tween.
	 * @param tweenTag            Tag identifying the tween to find.
	 * @return                    Pointer to the found UQuickTweenable instance, or nullptr if none found.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Find | By Tag", WorldContext = "worldContextObject"), Category = "QuickTween")
	static UQuickTweenable* QuickTweenFindTweenByTag(const UObject* worldContextObject, const FString& tweenTag);

	/**
	 * Find all active QuickTweens that match a specified predicate within the world context.
	 *
	 * This function searches through all active tweens managed by the tween manager
	 * associated with the provided \p worldContextObject and returns an array of
	 * UQuickTweenable instances that satisfy the given \p predicate.
	 *
	 * @param worldContextObject Context object used to locate the world that contains the tweens.
	 * @param predicate          The predicate function used to filter tweens.
	 * @return                   An array of UQuickTweenable pointers that match the predicate.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Find | All", WorldContext = "worldContextObject"), Category = "QuickTween")
	static TArray<UQuickTweenable*> QuickTweenFindAllTweensByPredicate(const UObject* worldContextObject, const FQuickConstTweenableAction& predicate);

	/**
	 * Kill all active QuickTweens within the specified world context.
	 *
	 * This function locates the tween manager for the provided \p worldContextObject
	 * and immediately kills/removes all active tweens. Use this to forcefully stop
	 * any running tweens (useful for cleanup, level transitions, or global reset).
	 *
	 * @param worldContextObject Context object used to locate the world that contains the tweens.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Kill | All", WorldContext = "worldContextObject"), Category = "QuickTween")
	static void QuickTweenKillAllTweens(const UObject* worldContextObject);

	/**
	 * Pause all active QuickTweens within the specified world context.
	 *
	 * This function finds the tween manager associated with \p worldContextObject
	 * and pauses all currently active tweens. Paused tweens retain their state and
	 * can be resumed later. This does not kill or destroy tweens.
	 *
	 * @param worldContextObject Context object used to locate the world that contains the tweens.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Pause | All", WorldContext = "worldContextObject"), Category = "QuickTween")
	static void QuickTweenPauseAllTweens(const UObject* worldContextObject);

	/**
	 * Play all QuickTweens within the specified world context.
	 *
	 * This function locates the tween manager for the provided \p worldContextObject
	 * and resumes all tweens that were previously paused. Active tweens that are not
	 * paused remain unaffected.
	 *
	 * @param worldContextObject Context object used to locate the world that contains the tweens.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Play | All", WorldContext = "worldContextObject"), Category = "QuickTween")
	static void QuickTweenPlayAllTweens(const UObject* worldContextObject);

	/**
	 * Reverse all active QuickTweens within the specified world context.
	 *
	 * This function finds the tween manager associated with \p worldContextObject
	 * and reverses the playback direction of all currently active tweens. Tweens
	 * that were playing forward will play backward, and vice versa.
	 *
	 * @param worldContextObject Context object used to locate the world that contains the tweens.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Reverse | All", WorldContext = "worldContextObject"), Category = "QuickTween")
	static void QuickTweenReverseAllTweens(const UObject* worldContextObject);

	/**
	 * Complete all active QuickTweens within the specified world context.
	 *
	 * This function locates the tween manager for the provided \p worldContextObject
	 * and immediately completes all active tweens, setting them to their end state.
	 * Completed tweens may be auto-killed based on their configuration.
	 *
	 * @param worldContextObject Context object used to locate the world that contains the tweens.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Complete | All", WorldContext = "worldContextObject"), Category = "QuickTween")
	static void QuickTweenCompleteAllTweens(const UObject* worldContextObject);

	/**
	 * Execute a specified action on all active QuickTweens within the given world context.
	 *
	 * This function locates the tween manager for the provided \p worldContextObject
	 * and performs the specified \p action on all active tweens. The action is defined
	 * by the FQuickTweenableAction enum (e.g., Pause, Play, Reverse, etc.).
	 *
	 * @param worldContextObject Context object used to locate the world that contains the tweens.
	 * @param action             The action to execute on all active tweens.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | All", WorldContext = "worldContextObject"), Category = "QuickTween")
	static void QuickTweenExecuteActionOnAllTweens(const UObject* worldContextObject,const FQuickTweenableAction& action);

	/**
	 * Execute a specified action on all active QuickTweens that match a given predicate within the world context.
	 *
	 * This function searches through all active tweens managed by the tween manager
	 * associated with the provided \p worldContextObject. For each tween that satisfies
	 * the given \p predicate, the specified \p action is executed.
	 *
	 * @param worldContextObject Context object used to locate the world that contains the tweens.
	 * @param predicate          The predicate function used to filter tweens.
	 * @param action             The action to execute on matching tweens.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Some", WorldContext = "worldContextObject"), Category = "QuickTween")
	static void QuickTweenExecuteActionByPredicate(const UObject* worldContextObject, const FQuickConstTweenableAction& predicate, const FQuickTweenableAction& action);
};
