# QuickTween

Lightweight, high-performance tweening utilities for Unreal Engine (Blueprint & C++).

QuickTween provides an easy-to-use API for interpolating values over time, with built-in easing, looping, and Blueprint-friendly helpers. It's intended for in-game animations, UI transitions, and any case where you need smooth value interpolation without the overhead of external tweening libraries.

---

## Features

- Simple, minimal API for creating tweens in C++ and Blueprints
- Built-in easing presets and custom `UCurveFloat` support
- Looping options (Restart, Yoyo, etc.) and configurable loop counts
- Safe lifetime handling with optional auto-kill on completion
- Works while paused (optional) and supports time scaling
- BlueprintType classes and BlueprintCallable functions for designer-friendly workflows

---

## Supported UE Versions

- Assumption: QuickTween targets Unreal Engine 5.5 and newer (UE5+).
- If you use a different engine version (UE4 or pre-release UE5), some API or build steps may need adjustment.

---

## Installation

1. Clone or copy the `QuickTween` folder into your project's `Plugins` folder:

   - Path: `YourProject/Plugins/QuickTween`

2. Regenerate project files (right-click the `.uproject` -> "Generate Visual Studio project files") or run your normal project file generation step.
3. Open the Editor and enable the plugin from `Edit > Plugins` (if it's not auto-enabled).
4. Restart the Editor when prompted.
5. Package-time: ensure the plugin is enabled for the target configuration in `Edit > Plugins` and included in packaging settings if required.

---

## Quick Start

These examples show the minimal usage patterns. See the API Overview below for full symbol names and headers.

Blueprints

1. Open your Blueprint and search the Palette for nodes starting with `QuickTween` (e.g. `Create Vector Tween`).
2. Provide `From` and `To` values or bind getter/setter functions exposed by your Blueprint.
3. Configure duration, easing, loops, and optionally check `Auto Play`.
4. Call `Play` or let it auto-play. Bind `OnComplete` to react when the tween finishes.

C++

- Typical usage (assumes you include the appropriate headers from `QuickTween/`):

```cpp
// Create a vector tween that interpolates between two getters and applies the result via setter
UQuickVectorTween* Tween = UQuickVectorTween::CreateTween(
    WorldContextObject,
    FNativeVectorGetter::CreateUObject(SomeObject, &ASomeActor::GetStart),
    FNativeVectorGetter::CreateUObject(SomeObject, &ASomeActor::GetEnd),
    FNativeVectorSetter::CreateUObject(SomeObject, &ASomeActor::SetLocation),
    1.0f /*duration*/,
    1.0f /*timeScale*/,
    EEaseType::CubicOut /*ease*/,
    nullptr /*easeCurve*/, 
    1 /*loops*/,
    ELoopType::Restart /*loop type*/, 
    TEXT("MyTweenTag"),
    true /*autoKill*/, 
    false /*playWhilePaused*/, 
    true /*autoPlay*/
);

// Optionally keep a pointer to control playback later
if (Tween)
{
    Tween->Pause();
    Tween->Play();
    Tween->Stop();
}
```

Notes:
- `CreateTween` returns `nullptr` if any required delegate is not bound.
- Tweens created with `NewObject` are owned by the Unreal GC; keep a pointer if you need to manipulate them later.

---

## API Overview (public surface)

The following lists the main public classes and symbols you will interact with. See header files in `Plugins/QuickTween/Source/QuickTween/Public/` for full documentation.

- `UQuickTweenBase` — Base tween class, provides lifecycle (Play, Pause, Stop), easing/time control, and common events.
- `UQuickVectorTween` — Vector-specific tween class. Use `CreateTween` to construct a tween from getters/setters.
- `FNativeVectorGetter` — Delegate type for retrieving `FVector` values at runtime.
- `FNativeVectorSetter` — Delegate type for applying `FVector` values.
- `EEaseType` — Enumeration with built-in easing presets (e.g., Linear, CubicIn, CubicOut, etc.).
- `ELoopType` — Enumeration for loop behavior (Restart, Yoyo, PingPong-like behaviors).

Common UFUNCTIONs

- `CreateTween(...)` — Static factory for creating typed tweens (e.g. `UQuickVectorTween::CreateTween`).
- `Play()` / `Pause()` / `Stop()` — Control playback.
- `SetTimeScale(float)` — Adjust playback speed.
- `SetEaseCurve(UCurveFloat*)` — Use a custom curve for easing.
- `GetCurrentValue()` / `GetStartValue()` / `GetEndValue()` — Query tween values.

---

## Configuration Options

- `duration` (float): Time, in seconds, for one iteration of the tween.
- `timeScale` (float): Multiplier applied to the tween's playback rate.
- `easeType` (EEaseType): Built-in easing presets.
- `easeCurve` (UCurveFloat*): Optional custom curve to control easing.
- `loops` (int32): Number of iterations (use `-1` or a provided constant for infinite loops if supported).
- `loopType` (ELoopType): How loops behave (Restart, Yoyo, etc.).
- `bShouldAutoKill` (bool): Whether the tween will be destroyed/GC'd automatically after completion.
- `bShouldPlayWhilePaused` (bool): Whether the tween should continue while the game is paused.
- `tweenTag` (FString): Optional tag to identify or query tweens.

---

## Blueprint Usage Notes & Gotchas

- Delegate binding: Blueprints expose getter/setter delegates. Make sure the delegates are properly bound before calling `CreateTween`.
- GC / Lifetime: Tweens are `UObject` instances; The QuickTweenManager will be responsible of holding a reference until Kill is called.
- Editor preview: Tweens that rely on runtime-only data will not preview in the editor unless you provide editor-time getters.
- Pure blueprint functions: Remember to store the tween if you need to bind events or control it after creation, as the pure function will create a new tween instance each time it's called.

---

## Tips & Troubleshooting

- Plugin not visible in Editor: Ensure the `QuickTween.uplugin` file exists in `Plugins/QuickTween` and that you regenerated project files.
- Nodes not appearing in Blueprints: Restart the editor and clear derived data if necessary; also check `Edit > Plugins` and confirm the plugin is enabled for the current project.
- Precision/Performance: For many simultaneous tweens, prefer batching setters into a single call per tick to reduce overhead.
- Packaging issues: Make sure plugin modules are included in the target packaging settings.

---

## Changelog

See `CHANGELOG.md` (not present yet). Use semantic versioning for releases. This file is a placeholder for release notes.

---

## Further Documentation

- Documentation: https://juanpablohdzm.github.io/QuickTweenDocs/

---

## Author

Juan Pablo Hernandez Mosti
