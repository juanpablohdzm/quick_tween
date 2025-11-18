// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utils/EaseType.h"
#include "Utils/LoopType.h"
#include "QuickTweenLibrary.generated.h"

class UQuickFloatTween;
class UQuickTweenable;
class UQuickTweenSequence;
class UQuickVectorTween;
class UQuickRotatorTween;

UENUM(BlueprintType)
enum class EQuickTweenSpace : uint8
{
	WorldSpace UMETA(DisplayName = "World Space"),
	LocalSpace UMETA(DisplayName = "Local Space")
};
/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_RetVal(FVector, FVectorGetter);
DECLARE_DYNAMIC_DELEGATE_RetVal(FRotator, FRotatorGetter);

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
	 * @return                    A newly created UQuickTweenSequence pointer (may be nullptr on failure).
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Sequence | Create | Make"), Category = "QuickTween")
	static UQuickTweenSequence* MakeQuickTweenSequence(
		UObject* worldContextObject,
		int32 loops = 1,
		ELoopType loopType = ELoopType::Restart,
		const FString& tweenTag = "",
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false);

	/**
	 * Create a vector tween that moves a SceneComponent to a target location.
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
	 * @return                    A UQuickVectorTween pointer controlling the movement.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Move To SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent"), Category = "QuickTween")
	static UQuickVectorTween* MoveTo_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false);

	/**
	 * Create a vector tween that scales a SceneComponent to a target scale.
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
	 * @return                    A UQuickVectorTween pointer controlling the scale tween.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Scale To SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent"), Category = "QuickTween")
	static UQuickVectorTween* ScaleTo_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false);

	/**
	 * Create a rotator tween that rotates a SceneComponent to a target rotator.
	 *
	 * @param worldContextObject  Context object used to find the world for the tween.
	 * @param component           The SceneComponent to rotate.
	 * @param to                  Target rotation.
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
	 * @return                    A UQuickRotatorTween pointer controlling the rotation.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween Rotate To SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent"), Category = "QuickTween")
	static UQuickRotatorTween* RotateTo_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false);



	/**
	 * Create a rotator tween that orients a SceneComponent to look at a target point.
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
	 * @return                    A UQuickRotatorTween pointer controlling the look-at rotation.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween LookAt SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent"), Category = "QuickTween")
	static UQuickRotatorTween* LookAt_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false);


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
	 * @param from                       Starting angle (degrees or radians depending on implementation).
	 * @param to                         Target angle (degrees or radians depending on implementation).
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
	 * @return                           A UQuickFloatTween pointer controlling the rotation, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Quick Tween RotateAroundPoint SceneComponent", meta = (Keywords = "Tween | Movement | SceneComponent"), Category = "QuickTween")
	static UQuickFloatTween* RotateAround_SceneComponent(
		UObject* worldContextObject,
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
		bool bShouldAutoKill = true,
		bool bShouldPlayWhilePaused = false);

	/**
	 * Find an active QuickTween by its tag within the world context.
	 *
	 * @param worldContextObject  Context object used to locate the world that contains the tween.
	 * @param tweenTag            Tag identifying the tween to find.
	 * @return                    Pointer to the found UQuickTweenable instance, or nullptr if none found.
	 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Tween | Find | By Tag"), Category = "QuickTween")
	static UQuickTweenable* FindTweenByTag(const UObject* worldContextObject, const FString& tweenTag);
};
