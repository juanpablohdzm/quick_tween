// QuickVectorTween.Spec.cpp

#include "Misc/AutomationTest.h"
#include "Tweens/QuickVectorTween.h"
#include "Utils/LoopType.h"

BEGIN_DEFINE_SPEC(QuickVectorTweenSpec, "Gameplay.QuickTween.Vector", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
UQuickVectorTween* Tween;
END_DEFINE_SPEC(QuickVectorTweenSpec)

void QuickVectorTweenSpec::Define()
{
	Describe("QuickTween Automation Tests - Vector", [this]()
	{
		BeforeEach([this]()
		{
			Tween = NewObject<UQuickVectorTween>(GetTransientPackage());
			Tween->AddToRoot(); // GC safety in tests
		});

		AfterEach([this]()
		{
			if (Tween)
			{
				Tween->RemoveFromRoot();
				Tween->ConditionalBeginDestroy();
				Tween = nullptr;
			}
		});

		It("Initializes and does not update unless playing", [this]()
		{
			FVector From(0,0,0), To(100,100,100);
			int32 UpdateCalls = 0;

			Tween->SetUp(From, To, [&UpdateCalls](const FVector&){ ++UpdateCalls; }, 1.0f);

			// No Play() yet
			Tween->Update(0.5f);

			TestEqual("Progress stays at 0 when not playing", Tween->GetProgress(), 0.0f);
			TestEqual("No callback invocations when not playing", UpdateCalls, 0);
			TestFalse("Not completed", Tween->GetIsCompleted());
		});

		It("Plays and updates towards halfway", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Current(0,0,0);

			Tween->SetUp(From, To, [&Current](const FVector& v){ Current = v; }, 1.0f);
			Tween->Play();
			Tween->Update(0.5f); // half the duration

			TestTrue("Tween progressed", Tween->GetProgress() > 0.0f);
			TestNearlyEqual("Value ~halfway", Current, FVector(50,50,50), 5.0f);
			TestFalse("Not completed at halfway", Tween->GetIsCompleted());
		});

		It("Clamps at the end and does not change afterwards", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Current(0,0,0);

			Tween->SetUp(From, To, [&Current](const FVector& v){ Current = v; }, 1.0f, 1.0f, EEaseType::Linear);
			Tween->Play();

			Tween->Update(1.1f); // overshoot to ensure finish

			TestTrue("Completed after overshoot", Tween->GetIsCompleted());
			TestNearlyEqual("Value clamped to target", Current, To, 1.0f);

			const FVector EndValue = Current;

			// Further updates should not move anything
			Tween->Update(0.5f);
			TestTrue("Value unchanged after completion", Current.Equals(EndValue, 1e-3f));
		});

		It("Handles slow time scaling (0.5x) correctly", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Current(0,0,0);

			Tween->SetUp(From, To, [&Current](const FVector& v){ Current = v; }, 1.0f, 0.5f, EEaseType::Linear);
			Tween->Play();

			Tween->Update(1.0f); // effective progress 0.5
			TestNearlyEqual("~middle at 1s with 0.5x", Current, To/2.0f, 2.0f);
			TestFalse("Not completed yet", Tween->GetIsCompleted());

			Tween->Update(1.0f); // reach end
			TestTrue("Completed after total 2s", Tween->GetIsCompleted());
			TestNearlyEqual("At target", Current, To, 1.0f);
		});

		It("Handles fast time scaling (2x) correctly", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Current(0,0,0);

			Tween->SetUp(From, To, [&Current](const FVector& v){ Current = v; }, 1.0f, 2.0f, EEaseType::Linear);
			Tween->Play();

			// 0.5s at 2x => finish
			Tween->Update(0.5f);
			TestTrue("Completed", Tween->GetIsCompleted());
			TestNearlyEqual("At target", Current, To, 1.0f);
		});

		It("Does not progress when TimeScale is zero", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Current(0,0,0);
			int32 Calls = 0;

			Tween->SetUp(From, To, [&Current, &Calls](const FVector& v){ ++Calls; Current = v; }, 1.0f, 0.0f, EEaseType::Linear);
			Tween->Play();

			Tween->Update(1.0f);

			TestEqual("Progress remains 0", Tween->GetProgress(), 0.0f);
			TestTrue("Value unchanged", Current.Equals(From, 1e-3f));
			TestFalse("Not completed", Tween->GetIsCompleted());
		});

		It("Zero duration completes immediately", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Current(0,0,0);
			int32 Calls = 0;

			Tween->SetUp(From, To, [&Current, &Calls](const FVector& v){ ++Calls; Current = v; }, 0.0f, 1.0f, EEaseType::Linear);
			Tween->Play();
			Tween->Update(0.0f); // tick once

			TestTrue("Completed instantly", Tween->GetIsCompleted());
			TestNearlyEqual("Value at target", Current, To, 1.0f);
			TestTrue("Callback fired at least once", Calls >= 1);
		});

		It("Single large delta clamps to end (overshoot)", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Current(0,0,0);

			Tween->SetUp(From, To, [&Current](const FVector& v){ Current = v; }, 1.0f);
			Tween->Play();
			Tween->Update(5.0f);

			TestTrue("Completed", Tween->GetIsCompleted());
			TestNearlyEqual("Value clamped to target", Current, To, 1.0f);
		});

		It("Many tiny updates accumulate to completion and progress is monotonic", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Current(0,0,0);

			Tween->SetUp(From, To, [&Current](const FVector& v){ Current = v; }, 1.0f, 1.0f, EEaseType::Linear);
			Tween->Play();

			float lastProgress = 0.0f;
			for (int i = 0; i < 200; ++i) // total dt 1.0
			{
				Tween->Update(0.005f);
				const float p = Tween->GetProgress();
				TestTrue("Progress is non-decreasing", p + 1e-6f >= lastProgress);
				lastProgress = p;
			}
			Tween->Update(0.01f); // ensure overshoot

			TestNearlyEqual("Value at target", Current, To, 1.0f);
			TestTrue("Completed after accumulation", Tween->GetIsCompleted());
		});

		It("Loops with Restart work (2 loops)", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Current(0,0,0);
			int8_t LoopsAmount = 2;

			Tween->SetUp(
				From, To,
				[&Current](const FVector& v){ Current = v; },
				1.0f, 1.0f, EEaseType::Linear,
				nullptr, LoopsAmount, ELoopType::Restart);

			// Loop 1
			Tween->Play();
			TestNearlyEqual("Start at From", Current, From, 1.0f);
			Tween->Update(0.5f);
			TestNearlyEqual("~halfway in loop 1", Current, FVector(50,50,50), 10.0f);
			Tween->Update(0.49f);
			TestNearlyEqual("At To after loop 1", Current, To, 2.0f);
			TestFalse("Not completed after first loop", Tween->GetIsCompleted());

			// Loop 2 (restart)
			Tween->Update(0.01f); // advance to next loop
			TestNearlyEqual("Restarted to From", Current, From, 10.0f);
			Tween->Update(1.0f);
			TestNearlyEqual("At To after loop 2", Current, To, 2.0f);

			TestTrue("Completed after all loops", Tween->GetIsCompleted());
		});

		It("Loops with PingPong work (forward then backward)", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Current(0,0,0);
			int8_t LoopsAmount = 2;

			Tween->SetUp(
				From, To,
				[&Current](const FVector& v){ Current = v; },
				1.0f, 1.0f, EEaseType::Linear,
				nullptr, LoopsAmount, ELoopType::PingPong);

			// Forward
			Tween->Play();
			Tween->Update(0.5f);
			TestNearlyEqual("~halfway forward", Current, FVector(50,50,50), 10.0f);
			Tween->Update(0.49f);
			TestNearlyEqual("Reached To", Current, To, 2.0f);
			Tween->Update(0.01f); // advance to next loop
			TestFalse("Not completed at mid ping-pong", Tween->GetIsCompleted());

			// Backward
			Tween->Update(0.5f);
			TestNearlyEqual("~halfway backward", Current, FVector(50,50,50), 10.0f);
			Tween->Update(0.49f);
			TestNearlyEqual("Returned to From", Current, From, 2.0f);
			Tween->Update(1.f); // ensure overshoot
			TestTrue("Completed after ping-pong loops", Tween->GetIsCompleted());
		});
	});
}
