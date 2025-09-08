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
			Tween->Update(0.5f);
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

		It("Reverse on Restart at loop 2/3: returns to 0 and completes (boundary-stepped)", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Cur(0,0,0);

			// dur=1, loops=3, Restart
			Tween->SetUp(From, To, [&Cur](const FVector& v){ Cur=v; },
			             1.0f, 1.0f, EEaseType::Linear, nullptr, 3, ELoopType::Restart);
			Tween->Play();

			// Enter loop 2 (t = 1.2): 1.0 + 0.2 (step each boundary)
			Tween->Update(1.0f);                 // finish loop 1 => Cur ~ To
			TestNearlyEqual("Loop1 end", Cur, To, 1.0f);

			Tween->Update(0.2f);                 // 0.2 into loop 2
			const FVector snapForward = Cur;

			// Reverse mid loop 2: should head back to 0 and COMPLETE when reaches From.
			Tween->Reverse();

			// Step back to the previous boundary (t=1.0)
			Tween->Update(0.2f);
			TestNearlyEqual("Back at loop1 boundary value (~To)", Cur, To, 1.0f);
			TestFalse("Not completed yet at 1.0", Tween->GetIsCompleted());

			// Step back to start (t=0.0)
			Tween->Update(1.0f);
			TestTrue("Completed when reached 0 after Reverse", Tween->GetIsCompleted());
			TestNearlyEqual("Ended at From after Reverse-to-zero", Cur, From, 1.0f);
		});

		It("Reverse on Restart in loop 3/3 mid-way: boundary steps 2.3 -> 2.0 -> 1.0 -> 0.0", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Cur(0,0,0);

			Tween->SetUp(From, To, [&Cur](const FVector& v){ Cur=v; },
			             1.0f, 1.0f, EEaseType::Linear, nullptr, 3, ELoopType::Restart);
			Tween->Play();

			// Advance to t=2.3: (1.0 + 1.0 + 0.3), stepping boundaries
			Tween->Update(1.0f);
			Tween->Update(1.0f);
			Tween->Update(0.3f);
			const FVector snap23 = Cur;
			TestTrue("> start and < end", Cur.X > 0.0f && Cur.X < 100.0f);

			// Reverse, then walk back across EACH boundary
			Tween->Reverse();

			Tween->Update(0.3f); // 2.3 -> 2.0
			TestTrue("Reached t=2.0 boundary", Cur.X >= 99.0f || Cur.Equals(To, 2.0f));

			Tween->Update(1.0f); // 2.0 -> 1.0
			TestTrue("Reached t=1.0 boundary", Cur.X >= 99.0f || Cur.Equals(To, 2.0f));

			Tween->Update(1.0f); // 1.0 -> 0.0
			TestTrue("Completed at zero after Reverse", Tween->GetIsCompleted());
			TestNearlyEqual("Ended at From", Cur, From, 1.0f);
		});

		It("Reverse at exact boundary (start of loop 2/3) then 1.0s back to zero completes", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Cur(0,0,0);

			Tween->SetUp(From, To, [&Cur](const FVector& v){ Cur=v; },
			             1.0f, 1.0f, EEaseType::Linear, nullptr, 3, ELoopType::Restart);
			Tween->Play();

			Tween->Update(1.0f); // exactly loop 1 end
			TestNearlyEqual("At To at 1.0", Cur, To, 1.0f);

			Tween->Reverse();     // now head to zero and complete there
			Tween->Update(1.0f);  // 1.0 back to zero

			TestTrue("Completed at zero after Reverse on boundary", Tween->GetIsCompleted());
			TestNearlyEqual("Ended at From", Cur, From, 1.0f);
		});

		It("Reverse honors TimeScale (0.5x): needs scaled time to return to zero and complete", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Cur(0,0,0);

			// TS=0.5 => elapsed advances at half speed
			Tween->SetUp(From, To, [&Cur](const FVector& v){ Cur=v; },
			             1.0f, 0.5f, EEaseType::Linear, nullptr, 3, ELoopType::Restart);
			Tween->Play();

			// Reach t=1.2 elapsed: need 2.4s of real time (2.0s to get to 1.0 + 0.4s to get to 1.2)
			Tween->Update(2.0f); // -> t=1.0
			TestNearlyEqual("At To (loop1 end) with timescale", Cur, To, 1.0f);
			Tween->Update(0.4f); // -> t=1.2

			Tween->Reverse();

			// Back to 1.0 needs 0.2 elapsed => 0.4s real time
			Tween->Update(0.4f);
			TestNearlyEqual("Back to boundary 1.0", Cur, To, 1.0f);
			TestFalse("Not done yet", Tween->GetIsCompleted());

			// Back to 0 needs 1.0 elapsed => 2.0s real time
			Tween->Update(2.0f);
			TestTrue("Completed at zero after Reverse with timescale", Tween->GetIsCompleted());
			TestNearlyEqual("Ended at From", Cur, From, 1.0f);
		});

		It("Reverse on single-loop (1/1) mid-way: goes back to 0 and completes instead of finishing at To", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Cur(0,0,0);

			Tween->SetUp(From, To, [&Cur](const FVector& v){ Cur=v; }, 1.0f, 1.0f, EEaseType::Linear, nullptr, 1, ELoopType::Restart);
			Tween->Play();

			Tween->Update(0.6f);   // forward 0.6
			TestTrue("Forward > halfway", Cur.X > 50.0f);

			Tween->Reverse();      // should cause completion when reaching 0
			Tween->Update(0.6f);   // 0.6 back to zero

			TestTrue("Completed at zero after Reverse", Tween->GetIsCompleted());
			TestNearlyEqual("Ended at From", Cur, From, 1.0f);
		});

		It("Reverse early on PingPong(3 loops): returns to 0 and completes at zero", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Cur(0,0,0);

			Tween->SetUp(From, To, [&Cur](const FVector& v){ Cur=v; },
			             1.0f, 1.0f, EEaseType::Linear, nullptr, 3, ELoopType::PingPong);
			Tween->Play();

			Tween->Update(0.25f);  // a bit into forward
			TestTrue("Forward started", Cur.X > 0.0f);

			Tween->Reverse();      // should head to zero and complete on arrival
			Tween->Update(0.25f);  // back to zero

			TestTrue("Completed at zero after Reverse", Tween->GetIsCompleted());
			TestNearlyEqual("Ended at From", Cur, From, 1.0f);
		});

		It("Reverse when already at start: completes immediately on next tick", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Cur(0,0,0);

			Tween->SetUp(From, To, [&Cur](const FVector& v){ Cur=v; }, 1.0f, 1.0f, EEaseType::Linear, nullptr, 2, ELoopType::Restart);
			Tween->Play();

			// Immediately reverse at t=0
			Tween->Reverse();
			Tween->Update(0.0f); // tick to apply

			TestTrue("Completed instantly since already at zero", Tween->GetIsCompleted());
			TestNearlyEqual("Still at From", Cur, From, 1.0f);
		});

		It("Reverse after completion is a no-op: remains completed and value unchanged", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Cur(0,0,0);

			Tween->SetUp(From, To, [&Cur](const FVector& v){ Cur=v; }, 1.0f, 1.0f, EEaseType::Linear, nullptr, 1, ELoopType::Restart);
			Tween->Play();
			Tween->Update(1.0f); // complete normally
			TestTrue("Completed normally", Tween->GetIsCompleted());
			const FVector EndVal = Cur;

			Tween->Reverse();     // should have no effect
			Tween->Update(0.5f);
			TestTrue("Still completed", Tween->GetIsCompleted());
			TestTrue("Value unchanged after reverse post-complete", Cur.Equals(EndVal, 1e-3f));
		});

		It("Reverse at loop boundary (PingPong even loop about to start): heads to To and completes", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Cur(0,0,0);

			// 2 loops pingpong: would normally end at start anyway,
			// but we verify Reverse path explicitly at boundary.
			Tween->SetUp(From, To, [&Cur](const FVector& v){ Cur=v; },
			             0.5f, 1.0f, EEaseType::Linear, nullptr, 2, ELoopType::PingPong);
			Tween->Play();

			Tween->Update(0.5f); // end of forward loop 1
			TestNearlyEqual("At To at boundary", Cur, To, 1.0f);

			Tween->Reverse();     // go to zero and complete there
			Tween->Update(0.5f);
			Tween->Update(0.5f);
			TestTrue("Completed at From after Reverse at boundary", Tween->GetIsCompleted());
			TestNearlyEqual("Ended at From", Cur, From, 1.0f);
		});

		It("Reverse multiple boundaries: loop2 mid -> back to loop1 end -> back to start; completes only at 0", [this]()
		{
			FVector From(0,0,0), To(100,100,100), Cur(0,0,0);

			Tween->SetUp(From, To, [&Cur](const FVector& v){ Cur=v; },
			             1.0f, 1.0f, EEaseType::Linear, nullptr, 3, ELoopType::Restart);
			Tween->Play();

			// enter loop 2 at t=1.4: 1.0 + 0.4 (hit boundary separately)
			Tween->Update(1.0f);
			Tween->Update(0.4f);
			TestTrue("In loop 2 forward", Cur.X > 0.0f && Cur.X < 100.0f);

			Tween->Reverse(); // should now head to zero and only complete when reach 0

			// step back to 1.0 boundary first
			Tween->Update(0.4f);
			TestNearlyEqual("At loop1 boundary again (~To)", Cur, To, 1.0f);
			TestFalse("Not completed yet at boundary", Tween->GetIsCompleted());

			// then to 0
			Tween->Update(1.0f);
			TestTrue("Completed ONLY when reaching 0", Tween->GetIsCompleted());
			TestNearlyEqual("Ended at From", Cur, From, 1.0f);
		});

	});
}
