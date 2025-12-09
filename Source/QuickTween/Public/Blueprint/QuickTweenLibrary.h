// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utils/EaseType.h"
#include "Utils/LoopType.h"
#include "Utils/CommonValues.h"
#include "QuickTweenLibrary.generated.h"

class UWidget;
class UQuickIntTween;
class UQuickColorTween;
class UQuickVector2DTween;
class UQuickTweenable;
class UQuickTweenSequence;
class UQuickVectorTween;
class UQuickRotatorTween;
class UQuickFloatTween;


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
	UFUNCTION(BlueprintPure, DisplayName = "Quick Tween Create Tween Vector", meta = (Keywords = "Tween | Vector", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintPure, DisplayName = "Quick Tween Create Tween Rotator", meta = (Keywords = "Tween | Rotator", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintPure, DisplayName = "Quick Tween Create Tween Float", meta = (Keywords = "Tween | Float", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintPure, DisplayName = "Quick Tween Create Tween Vector2D", meta = (Keywords = "Tween | Vector", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintPure, DisplayName = "Quick Tween Create Tween Color", meta = (Keywords = "Tween | Color", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintPure, DisplayName = "Quick Tween Create Tween Int", meta = (Keywords = "Tween | Int", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Move To SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
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

	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Move to Widget", meta = (Keywords = "Tween | Movement | Widget", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Move By SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Scale To SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Scale By SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Rotate To SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Rotate By SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween LookAt SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Rotate Around Point SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent", WorldContext = "worldContextObject"), Category = "QuickTween")
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
	 * Find an active QuickTween by its tag within the world context.
	 *
	 * @param worldContextObject  Context object used to locate the world that contains the tween.
	 * @param tweenTag            Tag identifying the tween to find.
	 * @return                    Pointer to the found UQuickTweenable instance, or nullptr if none found.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Find | By Tag", WorldContext = "worldContextObject"), DisplayName= "Quick Tween Query Find Tween By Tag", Category = "QuickTween")
	static UQuickTweenable* QuickTweenFindTweenByTag(const UObject* worldContextObject, const FString& tweenTag);
};
