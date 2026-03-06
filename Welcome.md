# Welcome to **QuickTween**

**QuickTween** is a lightweight, fast, and flexible tweening framework for **Unreal Engine**, designed to make value interpolation smooth and easy to use in both **C++** and **Blueprints**.

Tweening, short for “in-betweening,” is the process of smoothly transitioning a value over time.  
Instead of instantly jumping from one state to another, tweening calculates gradual steps in between, creating fluid motion or progression.

Tweening can be applied to many things:

- Moving an actor from one location to another
- Fading UI elements in or out
- Changing colors, scales, or rotations
- Animating camera transitions
- Adjusting gameplay variables over time

Rather than manually updating values every frame, a tweening system handles the timing, interpolation, easing, and completion events for you.

Why is Tweening Useful?

Tweening allows developers to achieve polished motion and feedback with minimal effort.  
Its benefits include:

- Smoother and more natural animations  
- Reduced boilerplate code for time-based changes  
- Easy control over timing and easing styles  
- Cleaner logic that separates animation behaviour from game logic  
- Faster iteration when experimenting with motion or transitions

Whether used for subtle UI animations or dynamic gameplay sequences, tweening helps make interactions feel responsive, polished, and engaging.  
This is where **QuickTween** comes in: providing an easy and flexible way to apply tweening inside Unreal Engine, both in C++ and Blueprints.

This guide covers installation for both Blueprint and C++ workflows. We will also explore examples to help you understand what **QuickTween** can do.

---

# **Requests**

If you need a feature that the current version doesn't have. Send me an email to "mosti.dev@gmail.com" with the subject **Request  - <Feature Name> **, and a description of what you would need. 

# **Bugs**
If you see something that is not working correctly, plese send me an email to "mosti.dev@gmail.com" with the subject ** Bug - <Descriptive Name> **, what engine version are you using, a description of the bug, callstack if possible, steps to reproduce and any other thing that could be helpful to fix it.

---

# **Installation**

First, let’s walk through how to install **QuickTween**.

After purchasing the plugin from the Fab Store, it will appear in your **Vault**.  
If it does not show up immediately, restart the launcher to refresh your library.

Once visible in your Vault, click **Install to Engine** and select the engine version that matches your target project.

When the installation completes:

1. Open your project.
2. Go to **Edit → Plugins**.
3. In the Installed category, locate **QuickTween**.
4. Enable it by checking the activation box.  
   The editor will prompt a restart—confirm to apply the change.

![Installation](instalation.png)

---

## **Blueprint Setup**

That’s all you need. Once the editor restarts, **QuickTween** is ready to use in Blueprints.

---

## **C++ Setup**

C++ requires one additional step.

1. Open your C++ project in your IDE of choice.
2. Navigate to your `Build.cs` file.
3. Add `"QuickTween"` to the **PublicDependencies** array, as shown below:

![Cpp Installation](cpp_instalation.png)

4. Save the file, then right-click your `.uproject` and select **Generate Visual Studio Project Files**.

![Generate VS Files](generate_vs_files.png)

If everything compiles successfully, you are ready to start using **QuickTween** in C++.

--

# **Introduction**

How does **QuickTween** work?

To understand QuickTween, it helps to know the key building blocks behind its framework.  
The system is designed around modular components that define how tweens behave and how values update over time.

---

## **UQuickTweenable**

`UQuickTweenable` is the common interface shared by all tween objects.  
It defines what you can do with any tween instance, such as:

- Controlling playback
- Querying its current state
- Configuring its behaviour

### Notes on Sequences

When a tween is part of a UQuickTweenSequence, only the owner is allowed to control it.    
If you are working with a tween that is not owned by a sequence, you can control it otherwise actions like "Play", "Pause" or "Complete", among others, will not be executed.  
This design ensures that ownership and execution order remain consistent when sequences manage multiple tweens.

---

## **UQuickTweenBase**

`UQuickTweenBase` is the foundational class for all specific tween types.  
It provides almost all the functionality required for interpolation while remaining generic enough to work with different data types (float, vector, rotator, and others).

Derived classes extend this base to define how their specific data type is updated.  
For example, a vector tween overrides the `ApplyAlphaValue` and `HandleOnComplete` functions to apply its value changes correctly.

Since `UQuickTweenBase` does not know about the details of the data type it manipulates, it is the responsibility of each derived class to:

- Store the values being tweened
- Validate them
- Apply interpolation logic appropriate to its type

### Note

In derived tween classes, you will notice that the system requires functions to retrieve both the **From** and **To** values.  
This design allows flexibility when supplying values at runtime.

- The **From** value is captured during the first update and remains constant for the entire tween.
- The **To** value is captured during the first update and remains constant for the entire tween.


## **UQuickTweenSequence**

`UQuickTweenSequence` is a container designed to run multiple tweens either in parallel or in order.  
It can manage both simple tween objects and nested sequences, allowing you to build behaviour ranging from straightforward transitions to deeply layered animation flows.

A key aspect of the system is ownership.  
Only the **outermost sequence** is responsible for coordinating all nested tweens and sub-sequences.  
Because of this, it is also the only sequence that can be directly controlled through the `UQuickTweenable` interface.

In other words, when working with sequences:

- You can nest sequences as deeply as you want
- Internal tweens and sub-sequences are automatically managed
- Control functions such as play, pause, or stop should be called only on the outermost sequence

This ensures predictable behaviour and prevents conflicting updates when working with complex tween structures.

### Note

For adding tweens we have two functions
- Append: allows you to add tweens one after the other. E.g.  A -> B.
- Join: allows you to run the tween to the last tween you appended. E.g. A & B at the same time.

## **UQuickTweenManager**

`UQuickTweenManager` is a singleton created alongside the `UWorld`, and it is destroyed when the `UWorld` is unloaded.  
Its purpose is to coordinate and drive all active tweens.

The manager:

- Ticks every tween each frame
- Supports tweening both during gameplay and when the game is paused
- Maintains a registry of all active tweens

When a tween (or the outermost tween in a sequence) is marked for removal, the manager clears it from the active list.  
Only the outermost tween is managed by the manager, other sub tweens are the outermost responsability.
This ensures proper lifecycle management, avoids orphaned tweens, and keeps performance predictable.

# **Development**

## **Ease**

QuickTween includes a wide range of easing functions to shape motion behaviour.  
These functions define how values accelerate or decelerate over time, giving animations more personality than simple linear interpolation.

QuickTween provides the following easing functions out of the box:

- **linear**

- **easeInSine**
- **easeOutSine**
- **easeInOutSine**

- **easeInQuad**
- **easeOutQuad**
- **easeInOutQuad**

- **easeInCubic**
- **easeOutCubic**
- **easeInOutCubic**

- **easeInQuart**
- **easeOutQuart**
- **easeInOutQuart**

- **easeInQuint**
- **easeOutQuint**
- **easeInOutQuint**

- **easeInExpo**
- **easeOutExpo**
- **easeInOutExpo**

- **easeInCirc**
- **easeOutCirc**
- **easeInOutCirc**

- **easeInBack**
- **easeOutBack**
- **easeInOutBack**

- **easeInElastic**
- **easeOutElastic**
- **easeInOutElastic**

- **easeInBounce**
- **easeOutBounce**
- **easeInOutBounce**

Users can also define their own easing curves, as long as they operate within the expected range of 0 to 1.


## **Blueprint Usage**

To support tween creation in Blueprints, QuickTween provides two Blueprint libraries:

- `UQuickTweenLibrary`
- `UQuickTweenLatentLibrary`

Both libraries expose the same set of functions.  
The difference lies in execution style:

- `UQuickTweenLibrary` offers immediate function calls. 
- `UQuickTweenLatentLibrary` provides latent equivalents, allowing Blueprint nodes to wait until one event occures.

This gives you flexibility depending on whether you want to trigger tweens instantly or integrate them directly into Blueprint execution flow.

All tweens provide event functions for Start, Update, Loop, Complete and OnKilled.

### **Simple BP Tween**
Note: You can enable auto play and the node after the assign is unecessary.
![Simple Tween](simple_tween.png)

### **Regular BP Sequence**
![Regular Tween](blueprint_widget.png)

### **Latent BP Sequence**
![Latent Tween](blueprint_latent.png)

### Note
All of things in UQuickTweenLibrary can be also be used in **C++**

## **C++ Example**

In C++, you can either use the non-latent functions from `UQuickTweenLibrary` or manually construct tweens yourself.

The following example demonstrates how to move an actor upward by 100 units over 2 seconds.

---

### **1. Get the object we want to tween**
```
	UStaticMeshComponent* MeshComp = MeshComponent;
```

### **2. Create the tween instance**
```
	UQuickVectorTween* Tween = UQuickVectorTween::CreateTween(
		/* worldContextObject*/ this,
		FNativeVectorGetter::CreateWeakLambda(MeshComp, [MeshComp = TWeakObjectPtr(MeshComp)](UQuickVectorTween*) { 
			
			if (!MeshComp.IsValid())
			{
				return FVector::Zero();
			}

			return MeshComp->GetComponentLocation(); 
		}),
		FNativeVectorGetter::CreateWeakLambda(MeshComp, [](UQuickVectorTween* tween) { 
			return tween->GetStartValue() + FVector(0.0f, 0.0f, 100.0f); // start value is set on start 	
		}),
		FNativeVectorSetter::CreateWeakLambda(MeshComp, [MeshComp = TWeakObjectPtr(MeshComp)](const FVector& NewLocation) { 
			if (!MeshComp.IsValid())
			{
				return FVector::Zero();
			}

			MeshComp->SetWorldLocation(NewLocation); 
		}),
		/*duration*/ 2.0f,
		/*time scale*/ 1.0f,
		EEaseType::Linear,
		/*ease curve*/ nullptr,
		/*loops*/ 1, // -1 for infinite loops
		ELoopType::Restart,
		TEXT("MoveUpTween"),
		/*bShouldAutoKill*/ true,
		/*bShouldPlayWhilePaused*/ false,
		/*bShouldAutoPlay*/ true,
		/*bShouldSnapToEndOnComplete*/ true
	);
```

### **Explanation**

- The first function retrieves the starting value.
- The second function retrieves the ending value.
- The third function applies the interpolated result.

All three functions use weak lambdas to ensure safety if the target component is destroyed before the tween finishes.

#### **Additional configuration notes**

- **World context:** Required to register the tween with the manager
- **Duration:** 2 seconds
- **Time scale:** Controls how fast or slow the tween progresses
- **Ease type:** Linear (default when no curve is provided)
- **Loops:** Runs once (use `-1` for infinite looping)

Once configured, the tween starts automatically, moves the actor upward over time, and cleans itself up when it finishes.

## **C++ Sequence Example**

If we want to put the previous tween in a sequence we can do the following:

### **1. Create the sequence**

To simplify usage, we can rely on `UQuickTweenLibrary`.  
This library function handles sequence creation internally and invokes the setup process for us, allowing tweens to be created with minimal boilerplate.
```
   UQuickTweenSequence* Sequence = UQuickTweenLibrary::QuickTweenCreateSequence(
		this,
		1,
		ELoopType::Restart,
		TEXT("ExampleSequence"),
		true,
		false);
```

### **2. Add all the tweens to the sequence**

The first tween added to a sequence can use either **Join** or **Append**, since there is no existing order yet.

After the first tween, ordering becomes important:

- **Append** will always place the new tween after the most recently appended one.
- **Join** allows tweens to run in parallel with the currently active segment.

This gives you control over whether tweens run sequentially or simultaneously as you build the sequence.
```
   Sequence->Append(Tween);
	Sequence->Join(Tween1); // This will execute Tween1 at the same time as Tween
	// Sequence->Append(Tween1); This will execute Tween1 after Tween completes
```

### **3. Call play**
```
   Sequence->Play();
```

# **Conclusion**

Thanks for using **QuickTween**, please if you have any question, comment or request contact me at mosti.dev@gmail.com