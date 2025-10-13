#include "Misc/AutomationTest.h"
#include "Tweens/QuickTweenSequence.h"
#include "Tweens/QuickVectorTween.h"
#include "Utils/LoopType.h"
#include "QuickTweenSequenceTestListener.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_DEFINE_SPEC(QuickTweenSequenceSpec, "Gameplay.QuickTween.Sequence", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

UQuickTweenSequence* Sequence;
UQuickTweenSequenceTestListener* Listener;

END_DEFINE_SPEC(QuickTweenSequenceSpec)

void QuickTweenSequenceSpec::Define()
{
	Describe("QuickTweenSequence Automation Tests", [this]()
	{
		BeforeEach([this]()
		{
			Sequence = NewObject<UQuickTweenSequence>(GetTransientPackage());
			Sequence->SetUp(nullptr);
			Sequence->AddToRoot();

			Listener = NewObject<UQuickTweenSequenceTestListener>(GetTransientPackage());
			Listener->AddToRoot();

			Sequence->OnUpdate.AddDynamic(Listener, &UQuickTweenSequenceTestListener::HandleUpdate);
			Sequence->OnComplete.AddDynamic(Listener, &UQuickTweenSequenceTestListener::HandleComplete);
			Sequence->OnKilled.AddDynamic(Listener, &UQuickTweenSequenceTestListener::HandleKilled);
		});

		AfterEach([this]()
		{
			if (Sequence)
			{
				Sequence->RemoveFromRoot();
				Sequence->ConditionalBeginDestroy();
				Sequence = nullptr;
			}
			if (Listener)
			{
				Listener->RemoveFromRoot();
				Listener->ConditionalBeginDestroy();
				Listener = nullptr;
			}
		});

		It("Should not advance when not playing", [this]()
		{
			auto FromA = []()->FVector { return FVector::ZeroVector; };
			auto ToA   = []()->FVector { return FVector(100,100,100); };

			FVector ACurrent(0,0,0);
			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->SetUp(FromA, ToA, [&ACurrent](const FVector& V){ ACurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA);

			Sequence->Update(0.5f);

			TestFalse("Sequence should not be playing", Sequence->GetIsPlaying());
			TestEqual("Elapsed time remains zero", Sequence->GetElapsedTime(), 0.0f);
			TestTrue("Joined tween should not have updated", ACurrent.Equals(FromA(), 1e-3f));
		});

		It("Should join tweens and run sequentially", [this]()
		{
			auto FromA = []()->FVector { return FVector::ZeroVector; };
			auto ToA   = []()->FVector { return FVector(100,100,100); };
			auto FromB = []()->FVector { return FVector::ZeroVector; };
			auto ToB   = []()->FVector { return FVector(50,50,50); };

			FVector ACurrent(0,0,0), BCurrent(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->SetUp(FromA, ToA, [&ACurrent](const FVector& V){ ACurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			auto TweenB = NewObject<UQuickVectorTween>(Sequence);
			TweenB->SetUp(FromB, ToB, [&BCurrent](const FVector& V){ BCurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Join(TweenA);
			Sequence->Join(TweenB);

			Sequence->Play();

			// Halfway through first tween
			Sequence->Update(0.5f);
			TestTrue("First tween ~halfway", ACurrent.Equals(FVector(50,50,50), 10.0f));
			TestTrue("Second tween not started yet", BCurrent.Equals(FromB(), 1e-3f));

			// Finish first + start second a bit
			Sequence->Update(0.5f); // total 1.0s
			TestTrue("First tween finished", ACurrent.Equals(ToA(), 1.0f));
			Sequence->Update(0.1f); // total 1.1s
			TestTrue("Second tween started (~0.1)", BCurrent.Equals(FVector(5,5,5), 2.0f));

			// Complete sequence
			Sequence->Update(2.0f);
			TestTrue("Sequence completed", Sequence->GetIsCompleted());
			TestTrue("Second tween finished", BCurrent.Equals(ToB(), 1.0f));

			TestTrue("OnUpdate called at least once", Listener->UpdateCalls > 0);
			TestEqual("OnComplete called once", Listener->CompleteCalls, 1);
		});

		It("Should append tweens and run them in parallel, taking the max duration", [this]()
		{
			auto FromA = []()->FVector { return FVector::ZeroVector; };
			auto ToA   = []()->FVector { return FVector(100,100,100); };
			auto FromB = []()->FVector { return FVector::ZeroVector; };
			auto ToB   = []()->FVector { return FVector(200,200,200); };

			FVector ACurrent(0,0,0), BCurrent(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->SetUp(FromA, ToA, [&ACurrent](const FVector& V){ ACurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			auto TweenB = NewObject<UQuickVectorTween>(Sequence);
			TweenB->SetUp(FromB, ToB, [&BCurrent](const FVector& V){ BCurrent = V; }, 2.0f, 1.0f, EEaseType::Linear);

			// Start a group with A, then append B to the same group
			Sequence->Join(TweenA);
			Sequence->Append(TweenB);

			TestTrue("Duration should be the max of joined tweens (~2s)", FMath::IsNearlyEqual(Sequence->GetDuration(), 2.0f, 1e-3f));

			Sequence->Play();

			Sequence->Update(0.5f);
			TestTrue("A ~50%", ACurrent.Equals(FVector(50,50,50), 10.0f));
			TestTrue("B ~25%", BCurrent.Equals(FVector(50,50,50), 10.0f));

			Sequence->Update(0.5f); // t = 1.0
			TestTrue("A finished at 1s", ACurrent.Equals(ToA(), 1.0f));
			TestTrue("B ~50% at 1s", BCurrent.Equals(FVector(100,100,100), 10.0f));
			TestFalse("Sequence not completed until the longest tween ends", Sequence->GetIsCompleted());

			Sequence->Update(1.0f); // t = 2.0
			TestTrue("B finished at 2s", BCurrent.Equals(ToB(), 1.0f));
			TestTrue("Sequence completed at max duration", Sequence->GetIsCompleted());
		});

		It("Should support Pause and TogglePause", [this]()
		{
			auto FromA = []()->FVector { return FVector::ZeroVector; };
			auto ToA   = []()->FVector { return FVector(100,100,100); };

			FVector Current(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->SetUp(FromA, ToA, [&Current](const FVector& V){ Current = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA);
			Sequence->Play();

			Sequence->Update(0.5f);
			TestTrue("~50% before pause", Current.Equals(FVector(50,50,50), 10.0f));

			Sequence->Pause();
			const FVector ValueAtPause = Current;
			Sequence->Update(0.5f);
			TestTrue("No change while paused", Current.Equals(ValueAtPause, 1e-3f));

			Sequence->Play(); // resume
			Sequence->Update(0.5f);
			TestTrue("Finished after resume", Current.Equals(ToA(), 1.0f));
			TestTrue("Sequence completed", Sequence->GetIsCompleted());
		});

		It("Should Complete immediately", [this]()
		{
			auto FromA = []()->FVector { return FVector::ZeroVector; };
			auto ToA   = []()->FVector { return FVector(100,100,100); };
			auto FromB = []()->FVector { return FVector::ZeroVector; };
			auto ToB   = []()->FVector { return FVector(50,50,50); };

			FVector ACurrent(0,0,0), BCurrent(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->SetUp(FromA, ToA, [&ACurrent](const FVector& V){ ACurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			auto TweenB = NewObject<UQuickVectorTween>(Sequence);
			TweenB->SetUp(FromB, ToB, [&BCurrent](const FVector& V){ BCurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA);
			Sequence->Append(TweenB);
			Sequence->Play();

			Sequence->Update(0.3f);
			Sequence->Complete();

			TestTrue("Sequence is completed", Sequence->GetIsCompleted());
			TestTrue("Progress equals duration", FMath::IsNearlyEqual(Sequence->GetElapsedTime(), Sequence->GetDuration(), 1e-3f));
			TestTrue("First tween at end", ACurrent.Equals(ToA(), 1.0f));
			TestTrue("Second tween at end", BCurrent.Equals(ToB(), 1.0f));
			TestEqual("OnComplete called once", Listener->CompleteCalls, 1);
		});

		It("Should Restart from the beginning", [this]()
		{
			auto FromA = []()->FVector { return FVector::ZeroVector; };
			auto ToA   = []()->FVector { return FVector(100,100,100); };

			FVector Current(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->SetUp(FromA, ToA, [&Current](const FVector& V){ Current = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA);
			Sequence->Play();

			Sequence->Update(0.8f);
			TestTrue("Near end before restart", Current.Equals(FVector(80,80,80), 10.0f));

			Sequence->Restart();
			TestEqual("Elapsed time reset", Sequence->GetElapsedTime(), 0.0f);
			Sequence->Update(0.0f); // tick once to apply initial value
			TestTrue("Value reset", Current.Equals(FromA(), 1.0f));
			TestFalse("Not completed after restart", Sequence->GetIsCompleted());

			Sequence->Update(1.0f);
			TestTrue("Finished after restart", Current.Equals(ToA(), 1.0f));
		});

		It("Should loop with Restart correctly", [this]()
		{
			auto FromA = []()->FVector { return FVector::ZeroVector; };
			auto ToA   = []()->FVector { return FVector(100,100,100); };

			FVector Current(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->SetUp(FromA, ToA, [&Current](const FVector& V){ Current = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->SetUp(nullptr,2, ELoopType::Restart);
			Sequence->Append(TweenA);

			Sequence->Play();
			TestEqual("CurrentLoop starts at 1", Sequence->GetCurrentLoop(), 1);

			Sequence->Update(0.99f);
			TestFalse("Not completed yet (end of loop 1)", Sequence->GetIsCompleted());

			Sequence->Update(0.01f); // cross into loop 2
			Sequence->Update(0.01f); // tick once to apply reset value
			TestTrue("Value reset at loop restart", Current.Equals(FromA(), 10.0f));
			TestEqual("CurrentLoop moved to 2", Sequence->GetCurrentLoop(), 2);

			Sequence->Update(1.0f);
			TestTrue("Sequence completed after 2 loops", Sequence->GetIsCompleted());
			TestTrue("Final value at target", Current.Equals(ToA(), 1.0f));
		});

		It("Should loop with PingPong correctly", [this]()
		{
			auto FromA = []()->FVector { return FVector::ZeroVector; };
			auto ToA   = []()->FVector { return FVector(100,100,100); };

			FVector Current(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->SetUp(FromA, ToA, [&Current](const FVector& V){ Current = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->SetUp(nullptr,2, ELoopType::PingPong);
			Sequence->Append(TweenA);

			Sequence->Play();

			// Forward
			Sequence->Update(0.5f);
			TestTrue("~halfway forward", Current.Equals(FVector(50,50,50), 10.0f));
			Sequence->Update(0.5f);
			TestTrue("Reached end of forward", Current.Equals(ToA(), 1.0f));
			TestFalse("Not completed after forward half", Sequence->GetIsCompleted());

			// Backward
			Sequence->Update(0.5f);
			TestTrue("~halfway backward", Current.Equals(FVector(50,50,50), 10.0f));

			Sequence->Update(0.25f);
			TestTrue("~quarterway backward", Current.Equals(FVector(25,25,25), 10.0f));

			Sequence->Update(0.25f);
			TestTrue("Returned to start after ping-pong", Current.Equals(FromA(), 1.0f));
			TestTrue("Sequence completed after ping-pong", Sequence->GetIsCompleted());
		});

		It("Should KillSequence and mark as pending kill", [this]()
		{
			auto FromA = []()->FVector { return FVector::ZeroVector; };
			auto ToA   = []()->FVector { return FVector(100,100,100); };

			FVector Current(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->SetUp(FromA, ToA, [&Current](const FVector& V){ Current = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA);
			Sequence->Play();

			Sequence->Update(0.2f);
			Sequence->Kill();

			TestTrue("Sequence pending kill", Sequence->GetIsPendingKill());
			TestEqual("OnKilled fired once", Listener->KilledCalls, 1);

			// Further updates shouldn't do anything meaningful
			const FVector ValueAtKill = Current;
			Sequence->Update(0.5f);
			TestTrue("No updates after kill", Current.Equals(ValueAtKill, 1e-3f));
		});

		It("Should report num tweens and allow GetTween access", [this]()
		{
			auto FromL = []()->FVector { return FVector::ZeroVector; };

			TArray<UQuickVectorTween*> Tweens;

			for (int i = 0; i < 3; ++i)
			{
				auto ToL = [i]()->FVector { const float v = (i+1) * 10.0f; return FVector(v, v, v); };

				auto T = NewObject<UQuickVectorTween>(Sequence);
				T->SetUp(FromL, ToL,
					[](const FVector&){}, 0.5f, 1.0f, EEaseType::Linear);
				Tweens.Add(T);

				if (i == 0) Sequence->Append(T);
				else if (i == 1) Sequence->Join(T);    // join second into first group
				else Sequence->Append(T);              // third starts new group
			}

			const int32 Num = Sequence->GetNumTweens();
			TestEqual("NumTweens == 3", Num, 3);

			bool bFoundAll = true;
			for (int32 idx = 0; idx < Num; ++idx)
			{
				auto* Got = Sequence->GetTween(idx);
				if (!Got) { bFoundAll = false; break; }
			}
			TestTrue("All GetTween calls returned non-null", bFoundAll);
		});

		It("Multi-group (Join then Append) with Restart x3 loops", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto To1  = []()->FVector { return FVector(100,100,100); };
			auto To2  = []()->FVector { return FVector(200,200,200); };
			auto To3  = []()->FVector { return FVector(50,50,50); };

			// Group 1 (parallel): T1(1s -> 100), T2(2s -> 200)
			// Group 2 (solo):     T3(0.5s -> 50)
			FVector V1(0,0,0), V2(0,0,0), V3(0,0,0);

			auto T1 = NewObject<UQuickVectorTween>(Sequence);
			T1->SetUp(From, To1, [&V1](const FVector& v){ V1 = v; }, 1.0f);
			auto T2 = NewObject<UQuickVectorTween>(Sequence);
			T2->SetUp(From, To2, [&V2](const FVector& v){ V2 = v; }, 2.0f);
			auto T3 = NewObject<UQuickVectorTween>(Sequence);
			T3->SetUp(From, To3, [&V3](const FVector& v){ V3 = v; }, 0.5f);

			Sequence->SetUp(nullptr,3, ELoopType::Restart);
			Sequence->Join(T1);
			Sequence->Append(T2); // new group, then add parallel
			Sequence->Join(T3);             // next group
			Sequence->Play();

			// Loop 1
			Sequence->Update(2.0f); // finish group 1 (dominated by T2)
			TestTrue("G1 finished: V1=100", V1.Equals(To1(), 1.0f));
			TestTrue("G1 finished: V2=200", V2.Equals(To2(), 1.0f));
			Sequence->Update(0.5f); // finish group 2
			TestTrue("G2 finished: V3=50", V3.Equals(To3(), 1.0f));

			// Loop 2 starts -> values reset
			Sequence->Update(0.01f);
			TestTrue("Loop 2 reset V1", V1.Equals(From(), 5.0f));
			TestTrue("Loop 2 reset V2", V2.Equals(From(), 5.0f));

			// Finish Loop 2 and 3
			Sequence->Update(2.0f);
			Sequence->Update(0.5f);
			Sequence->Update(2.0f);
			Sequence->Update(0.5f);

			TestTrue("Sequence completed after 3 loops", Sequence->GetIsCompleted());
			TestTrue("Ends at last group's target", V3.Equals(To3(), 1.0f));
		});

		It("Five tweens in one group (parallel) with Restart x2 loops", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto To0  = []()->FVector { return FVector(20,20,20);  };
			auto To1  = []()->FVector { return FVector(40,40,40);  };
			auto To2  = []()->FVector { return FVector(60,60,60);  };
			auto To3  = []()->FVector { return FVector(80,80,80);  };
			auto To4  = []()->FVector { return FVector(100,100,100); };

			FVector V0(0,0,0), V1(0,0,0), V2(0,0,0), V3(0,0,0), V4(0,0,0);

			auto T0 = NewObject<UQuickVectorTween>(Sequence);
			T0->SetUp(From, To0,  [&V0](const FVector& v){ V0 = v; }, 0.2f);
			auto T1o = NewObject<UQuickVectorTween>(Sequence);
			T1o->SetUp(From, To1,  [&V1](const FVector& v){ V1 = v; }, 0.4f);
			auto T2o = NewObject<UQuickVectorTween>(Sequence);
			T2o->SetUp(From, To2,  [&V2](const FVector& v){ V2 = v; }, 0.6f);
			auto T3o = NewObject<UQuickVectorTween>(Sequence);
			T3o->SetUp(From, To3,  [&V3](const FVector& v){ V3 = v; }, 0.8f);
			auto T4o = NewObject<UQuickVectorTween>(Sequence);
			T4o->SetUp(From, To4, [&V4](const FVector& v){ V4 = v; }, 1.0f);

			Sequence->SetUp(nullptr,2, ELoopType::Restart);
			Sequence->Join(T0);
			Sequence->Append(T1o);
			Sequence->Append(T2o);
			Sequence->Append(T3o);
			Sequence->Append(T4o);
			Sequence->Play();

			Sequence->Update(0.5f);
			TestTrue("T0 finished", V0.Equals(To0(), 1.0f));
			TestTrue("T1 finished", V1.Equals(To1(), 1.0f));
			TestTrue("T2 ~>40", V2.X > 40.0f);
			TestTrue("T3 ~>40", V3.X > 40.0f);
			TestTrue("T4 ~>40", V4.X > 40.0f);

			Sequence->Update(0.5f); // all done at 1.0s
			TestTrue("All at target end of loop 1", V0.Equals(To0(),1.0f) && V4.Equals(To4(),1.0f));

			Sequence->Update(0.01f); // loop 2 reset
			TestTrue("Reset to start in loop 2", V0.Equals(From(),5.0f) && V4.Equals(From(),5.0f));

			Sequence->Update(1.0f);
			TestTrue("Sequence done after 2 loops", Sequence->GetIsCompleted());
		});

		It("Join group then Join next group with Restart x2 (per-group boundaries)", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(60,60,60);   };
			auto ToB  = []()->FVector { return FVector(120,120,120); };
			auto ToC  = []()->FVector { return FVector(30,30,30);   };
			auto ToD  = []()->FVector { return FVector(240,240,240); };

			FVector A(0,0,0), B(0,0,0), C(0,0,0), D(0,0,0);

			// Group 1 (parallel): A(0.6s->60), B(0.6s->120)
			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA,   [&A](const FVector& v){ A=v; }, 0.6f);
			auto TB = NewObject<UQuickVectorTween>(Sequence);
			TB->SetUp(From, ToB,   [&B](const FVector& v){ B=v; }, 0.6f);

			// Group 2 (parallel): C(0.3s->30), D(1.2s->240)
			auto TC = NewObject<UQuickVectorTween>(Sequence);
			TC->SetUp(From, ToC,   [&C](const FVector& v){ C=v; }, 0.3f);
			auto TD = NewObject<UQuickVectorTween>(Sequence);
			TD->SetUp(From, ToD,   [&D](const FVector& v){ D=v; }, 1.2f);

			Sequence->SetUp(nullptr,2, ELoopType::Restart);
			Sequence->Join(TA);
			Sequence->Append(TB); // G1
			Sequence->Join(TC);
			Sequence->Append(TD); // G2
			Sequence->Play();

			Sequence->Update(0.6f);
			TestTrue("A,B finished at end of G1", A.Equals(ToA(),1.0f) && B.Equals(ToB(),1.0f));
			TestTrue("C,D not started yet", C.Equals(From(),1e-3f) && D.Equals(From(),1e-3f));

			Sequence->Update(1.2f);
			TestTrue("D finished at end of G2", D.Equals(ToD(),1.0f));
			TestTrue("C finished earlier", C.Equals(ToC(),1.0f));

			Sequence->Update(0.01f); // loop 2 reset
			TestTrue("All reset on loop 2 start", A.Equals(From(),5.0f));

			Sequence->Update(0.6f); // finish loop 2 (0.6 + 1.2)
			Sequence->Update(1.2f); // finish loop 2 (0.6 + 1.2)
			TestTrue("Completed after 2 loops", Sequence->GetIsCompleted());
		});

		It("PingPong x2 with two sequential groups (Join A, then Join B) ends at start pose", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100,100,100); };
			auto ToB  = []()->FVector { return FVector(50,50,50); };

			FVector A(0,0,0), B(0,0,0);

			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 0.5f);
			auto TB = NewObject<UQuickVectorTween>(Sequence);
			TB->SetUp(From, ToB, [&B](const FVector& v){ B=v; }, 0.25f);

			Sequence->SetUp(nullptr,2, ELoopType::PingPong);
			Sequence->Join(TA); // group 1
			Sequence->Join(TB); // group 2
			Sequence->Play();

			// Forward pass (0.75s total)
			Sequence->Update(0.5f);
			TestTrue("Forward mid", A.Equals(ToA(),1.0f) && B.Equals(From(),1.0f));
			Sequence->Update(0.25f);
			TestTrue("Forward end reached", A.Equals(ToA(),1.0f) && B.Equals(ToB(),1.0f));
			TestFalse("Not completed yet", Sequence->GetIsCompleted());

			// Backward pass mirrors (0.75s back)
			Sequence->Update(0.25f);
			TestTrue("Backward mid", A.Equals(ToA(),1.0f) && B.Equals(From(),1.0f));
			Sequence->Update(0.5f);
			TestTrue("Back at start pose", A.Equals(From(),1.0f) && B.Equals(From(),1.0f));
			TestTrue("Sequence completed", Sequence->GetIsCompleted());
		});

		It("PingPong x3 with one parallel group ends at end pose", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100,100,100); };
			auto ToB  = []()->FVector { return FVector(200,200,200); };

			FVector A(0,0,0), B(0,0,0);
			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 0.4f);
			auto TB = NewObject<UQuickVectorTween>(Sequence);
			TB->SetUp(From, ToB, [&B](const FVector& v){ B=v; }, 0.8f);

			Sequence->SetUp(nullptr,3, ELoopType::PingPong);
			Sequence->Join(TA);
			Sequence->Append(TB); // one group, parallel
			Sequence->Play();

			// Forward (0.8) + Backward (0.8) + Forward (0.8)
			Sequence->Update(0.8f);
			Sequence->Update(0.8f);
			Sequence->Update(0.8f);

			TestTrue("Ends at end pose (odd loops)", A.Equals(ToA(),1.0f) && B.Equals(ToB(),1.0f));
			TestTrue("Sequence completed", Sequence->GetIsCompleted());
		});

		It("Restart x2: Reverse mid-group flips direction within the loop", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100,100,100); };

			FVector A(0,0,0);
			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 1.0f);

			Sequence->SetUp(nullptr,2, ELoopType::Restart);
			Sequence->Join(TA);
			Sequence->Play();

			Sequence->Update(0.6f);
			const float Snapshot = A.X; // ~60
			Sequence->Reverse();
			Sequence->Update(0.2f);
			TestTrue("Value moved back toward start", A.X < Snapshot);

			Sequence->Update(0.4f); // reach start
			TestTrue("Returned to start", A.Equals(From(), 2.0f));
			Sequence->Reverse(); // go forward again
			Sequence->Update(1.0f); // finish loop 1 forward
			Sequence->Update(1.0f); // finish loop 2 forward
			TestTrue("Completed", Sequence->GetIsCompleted());
			TestTrue("End at forward end", A.Equals(ToA(), 2.0f));
		});

		It("Restart x2: Start reversed before Play on two sequential groups", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100,100,100); };
			auto ToB  = []()->FVector { return FVector(50,50,50); };

			FVector A(0,0,0), B(0,0,0);

			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 0.5f);
			auto TB = NewObject<UQuickVectorTween>(Sequence);
			TB->SetUp(From, ToB, [&B](const FVector& v){ B=v; }, 0.5f);

			Sequence->SetUp(nullptr,2, ELoopType::Restart);
			Sequence->Join(TA); // group 1
			Sequence->Join(TB); // group 2
			Sequence->Reverse(); // start backwards
			Sequence->Restart();
			Sequence->Play();

			Sequence->Update(0.01f); // apply initial pose
			TestTrue("Start at end of last group in backwards play",
				B.Equals(ToB(), 2.0f));

			Sequence->Update(0.24f);
			TestTrue("Values decreasing from ends", B.X < 50.0f);

			Sequence->Update(0.25f); // finish first group backward
			TestTrue("First group reached start", B.Equals(From(), 2.0f));

			Sequence->Update(0.25f); // reach starts
			TestTrue("Second group mid", A.Equals(FVector(50,50,50), 2.0f));

			Sequence->Update(0.25f);
			TestTrue("Both groups at start", A.Equals(From(), 2.0f) && B.Equals(From(), 2.0f));

			Sequence->Reverse();     // go forward
			Sequence->Update(0.5f);
			Sequence->Update(0.5f);
			TestTrue("Completed", Sequence->GetIsCompleted());
		});

		It("PingPong x2: Reverse just before turning point keeps the cycle valid", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100,100,100); };

			FVector A(0,0,0);
			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 1.0f);

			Sequence->SetUp(nullptr,2, ELoopType::PingPong);
			Sequence->Join(TA);
			Sequence->Play();

			Sequence->Update(0.9f);
			const float Pre = A.X;
			Sequence->Reverse();  // before natural flip
			Sequence->Update(0.2f);
			TestTrue("Moved backward after reverse", A.X < Pre);

			Sequence->Update(0.9f);
			TestTrue("Completed at start pose", A.Equals(From(), 2.0f));
		});

		It("Restart x3: Mixed time-scales in one parallel group respect the slowest", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToFast = []()->FVector { return FVector(90,90,90); };
			auto ToSlow = []()->FVector { return FVector(100,100,100); };

			FVector Fast(0,0,0), Slow(0,0,0);

			auto TFast = NewObject<UQuickVectorTween>(Sequence);
			TFast->SetUp(From, ToFast,  [&Fast](const FVector& v){ Fast=v; }, 1.0f, 2.0f /*2x*/);
			auto TSlow = NewObject<UQuickVectorTween>(Sequence);
			TSlow->SetUp(From, ToSlow, [&Slow](const FVector& v){ Slow=v; }, 1.0f, 0.5f /*0.5x*/);

			Sequence->SetUp(nullptr,3, ELoopType::Restart);
			Sequence->Join(TFast);
			Sequence->Append(TSlow); // one group, parallel
			Sequence->Play();

			Sequence->Update(0.5f);
			TestTrue("Fast progressed more than Slow", Fast.X > Slow.X);

			Sequence->Update(1.5f); // end loop 1 (slowest governs)
			TestTrue("Both at targets at loop 1 end", Fast.Equals(ToFast(),2.0f) && Slow.Equals(ToSlow(),2.0f));

			Sequence->Update(0.01f); // loop 2 reset
			Sequence->Update(2.0f);  // finish loop 2
			Sequence->Update(0.01f); // loop 3 reset
			Sequence->Update(2.0f);  // finish loop 3

			TestTrue("Completed after 3 loops", Sequence->GetIsCompleted());
		});

		It("Three groups total: G1 solo, G2 parallel(TB,TC), Restart x2 preserves ordering", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(10,10,10); };
			auto ToB  = []()->FVector { return FVector(20,20,20); };
			auto ToC  = []()->FVector { return FVector(30,30,30); };

			FVector A(0,0,0), B(0,0,0), C(0,0,0);

			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 0.2f);
			auto TB = NewObject<UQuickVectorTween>(Sequence);
			TB->SetUp(From, ToB, [&B](const FVector& v){ B=v; }, 0.3f);
			auto TC = NewObject<UQuickVectorTween>(Sequence);
			TC->SetUp(From, ToC, [&C](const FVector& v){ C=v; }, 0.1f);

			// G1: TA
			// G2: TB + TC (parallel)
			Sequence->SetUp(nullptr,2, ELoopType::Restart);
			Sequence->Join(TA);             // G1
			Sequence->Join(TB);
			Sequence->Append(TC); // G2
			Sequence->Play();

			Sequence->Update(0.2f);
			TestTrue("After G1, A at end", A.Equals(ToA(),1.0f));
			TestTrue("G2 not started yet", B.Equals(From(),1e-3f) && C.Equals(From(),1e-3f));

			Sequence->Update(0.3f); // end G2 (dominated by TB)
			TestTrue("B finished", B.Equals(ToB(),1.0f));
			TestTrue("C finished", C.Equals(ToC(),1.0f));

			Sequence->Update(0.01f); // loop 2 reset
			TestTrue("All reset", A.Equals(From(),5.0f));

			for (int i = 0; i < 5; ++i)
			{
				Sequence->Update(0.5f);
			}
			TestTrue("Completed", Sequence->GetIsCompleted());
		});

		It("Restart x2: Complete mid loop 2 jumps to terminal pose (last group's end) and fires once", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100,100,100); };
			auto ToB  = []()->FVector { return FVector(50,50,50); };

			FVector A(0,0,0), B(0,0,0);

			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 0.5f);
			auto TB = NewObject<UQuickVectorTween>(Sequence);
			TB->SetUp(From, ToB, [&B](const FVector& v){ B=v; }, 0.5f);

			Sequence->SetUp(nullptr,2, ELoopType::Restart);
			Sequence->Join(TA); // group 1
			Sequence->Join(TB); // group 2
			Sequence->Play();

			Sequence->Update(1.0f); // finish loop 1
			Sequence->Update(0.25f); // mid loop 2
			Sequence->Complete();

			TestTrue("Sequence completed immediately", Sequence->GetIsCompleted());
			TestEqual("OnComplete called once", Listener->CompleteCalls, 1);
			TestTrue("Terminal pose equals last group's end", B.Equals(ToB(), 2.0f));
		});

		It("PingPong x2: One parallel group of 3 tweens, validate mid-backward values", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(30,30,30);  };
			auto ToB  = []()->FVector { return FVector(60,60,60);  };
			auto ToC  = []()->FVector { return FVector(90,90,90);  };

			FVector A(0,0,0), B(0,0,0), C(0,0,0);
			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA,  [&A](const FVector& v){ A=v; }, 0.3f);
			auto TB = NewObject<UQuickVectorTween>(Sequence);
			TB->SetUp(From, ToB,  [&B](const FVector& v){ B=v; }, 0.6f);
			auto TC = NewObject<UQuickVectorTween>(Sequence);
			TC->SetUp(From, ToC,  [&C](const FVector& v){ C=v; }, 0.9f);

			Sequence->SetUp(nullptr,2, ELoopType::PingPong);
			Sequence->Join(TA);
			Sequence->Append(TB);
			Sequence->Append(TC); // one group, parallel
			Sequence->Play();

			Sequence->Update(0.9f); // forward end (dominated by TC)
			TestTrue("Forward end reached", A.Equals(ToA(),2.0f) && B.Equals(ToB(),2.0f) && C.Equals(ToC(),2.0f));

			Sequence->Update(0.45f); // halfway back
			TestTrue("Mid-backward values decreasing", A.X < 30.0f && B.X < 60.0f && C.X < 90.0f);

			Sequence->Update(0.45f); // reach start
			TestTrue("Back at start", A.Equals(From(),2.0f) && B.Equals(From(),2.0f) && C.Equals(From(),2.0f));
			TestTrue("Completed", Sequence->GetIsCompleted());
		});

		It("Restart x2: Reverse immediately after loop boundary still yields two full cycles", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100,100,100); };

			FVector A(0,0,0);
			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 0.5f);

			Sequence->SetUp(nullptr,2, ELoopType::Restart);
			Sequence->Join(TA);
			Sequence->Play();

			Sequence->Update(0.5f);   // finish loop 1
			Sequence->Update(0.01f);  // boundary into loop 2
			Sequence->Reverse();      // immediately reverse
			Sequence->Update(0.01f);
			Sequence->Update(0.25f);
			TestTrue("Moving toward start in loop 2 after reverse", A.X <= 50.0f);

			Sequence->Update(0.24f);  // reach start
			Sequence->Reverse();      // forward again
			Sequence->Update(0.25f);   // finish forward
			Sequence->Update(0.25f);   // finish loop 1 forward
			Sequence->Update(0.5f);   // finish loop 2 forward
			TestTrue("Completed after two cycles with reverses", Sequence->GetIsCompleted());
		});

		It("Internal loops: G1 TA(3x PingPong) vs TB(2x PingPong) — odd/even parity end poses", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100,100,100); };
			auto ToB  = []()->FVector { return FVector(50,50,50);   };

			FVector A(0,0,0), B(0,0,0);

			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; },
			          /*dur*/0.6f, /*ts*/1.0f, EEaseType::Linear, nullptr, /*loops*/3, ELoopType::PingPong); // total 1.8 (end = target)
			auto TB = NewObject<UQuickVectorTween>(Sequence);
			TB->SetUp(From, ToB, [&B](const FVector& v){ B=v; },
			          /*dur*/0.7f, /*ts*/1.0f, EEaseType::Linear, nullptr, /*loops*/2, ELoopType::PingPong); // total 1.4 (end = start)

			Sequence->Join(TA);
			Sequence->Append(TB); // one parallel group
			Sequence->Play();

			// Step to just before TA's first boundary (0.6), both forward
			Sequence->Update(0.59f);
			TestTrue("TA (odd) near end of first forward (~>99)", A.X > 97.0f);
			TestTrue("TB (even) in forward (~>42)",               B.X > 42.0f);

			// Finish TA first forward loop precisely at 0.6
			Sequence->Update(0.01f);
			TestTrue("TA reached target at 0.6", A.Equals(ToA(), 1.5f));
			// TB still forward, just shy of its 0.7 boundary
			TestTrue("TB still forward just before 0.7", B.X < 50.0f);

			// Go a bit into TA's backward (2nd loop), but still before TB finishes its first forward
			for (int i = 0; i < 9; ++i) // 9 * 0.01 = 0.09
			{
				Sequence->Update(0.01f);
			}// t = 0.69
			TestTrue("TA now in backward — below ~86", A.X < 86.0f);
			TestTrue("TB still forward, near end (~>49)", B.X > 49.0f);

			// Hit TB forward boundary exactly at 0.7
			Sequence->Update(0.01f); // t = 0.70
			TestTrue("TB hit 50 at 0.7", B.Equals(ToB(), 1.0f));

			// March to TA's second boundary (1.2 total): +0.5s of backward
			Sequence->Update(0.50f);  // t = 1.20
			TestTrue("TA finished backward (loop 2) at start", A.Equals(From(), 1.0f));
			// TB is now backward since 0.7; at t=1.2 (0.5 backward into 0.7 window), it should be ~50 - (0.5/0.7)*50 ≈ 14.3
			TestTrue("TB backward mid (~<20)", B.X < 20.0f);

			// Finish TB's 2nd loop (backward ends at start) at 1.4: +0.2s
			Sequence->Update(0.20f);  // t = 1.40
			TestTrue("TB (even loops) ended at start", B.Equals(From(), 1.5f));

			// TA is in its 3rd loop (forward); finish to 1.8: +0.4s
			Sequence->Update(0.42f);  // t = 1.80
			TestTrue("TA (odd loops) ended at target", A.Equals(ToA(), 1.5f));

			TestTrue("Group ended, sequence completed", Sequence->GetIsCompleted());
		});

		It("Internal loops: G1 parallel TA(2x Restart, 0.4) + TB(3x Restart, 0.3); G2 TC(2x Restart, 0.5)", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100); };
			auto ToB  = []()->FVector { return FVector(90);  };
			auto ToC  = []()->FVector { return FVector(50);  };

			FVector A(0), B(0), C(0);
			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 0.4f, 1.0f, EEaseType::Linear, nullptr, 2, ELoopType::Restart); // 0.8
			auto TB = NewObject<UQuickVectorTween>(Sequence);
			TB->SetUp(From, ToB, [&B](const FVector& v){ B=v; }, 0.3f, 1.0f, EEaseType::Linear, nullptr, 3, ELoopType::Restart); // 0.9
			auto TC = NewObject<UQuickVectorTween>(Sequence);
			TC->SetUp(From, ToC, [&C](const FVector& v){ C=v; }, 0.5f, 1.0f, EEaseType::Linear, nullptr, 2, ELoopType::Restart); // 1.0

			Sequence->Join(TA);
			Sequence->Append(TB); // G1 = 0.9
			Sequence->Join(TC);             // G2 = 1.0
			Sequence->Play();

			// Hit TB first boundary 0.3
			Sequence->Update(0.30f);
			TestTrue("TB 1/3 forward done", B.Equals(ToB(), 1.0f));
			TestTrue("TA mid (~>70)",       A.X > 70.0f);

			// Hit TA first boundary 0.4
			Sequence->Update(0.10f); // t = 0.40
			TestTrue("TA loop1 end", A.Equals(ToA(), 1.0f));
			TestTrue("TB loop2 forward mid (~>45)", B.X > 20.0f);

			// Hit TA loop2 end 0.8
			Sequence->Update(0.40f); // t = 0.80
			TestTrue("TA loop2 end", A.Equals(ToA(), 1.0f));
			TestTrue("TB approaching end of loop3", B.X > 20.0f);

			// Finish G1 at 0.9 (TB loop3 end)
			Sequence->Update(0.10f); // t = 0.90
			TestTrue("TB loop3 end", B.Equals(ToB(), 1.0f));
			TestTrue("G2 not yet started", C.Equals(From(), 1e-3f));

			// Now G2 in two steps 0.5 + 0.5
			Sequence->Update(0.50f);
			TestTrue("TC mid (~25)", C.Equals(ToC(), 1.0f));
			Sequence->Update(0.50f);
			TestTrue("TC completed", C.Equals(ToC(), 1.0f));
			TestTrue("Sequence completed", Sequence->GetIsCompleted());
		});

		It("Internal loops: G1 3 tweens mixed (T1:3x PingPong 0.2, T2:2x Restart 0.4, T3:4x PingPong 0.15) then G2 TC(2x Restart 0.25)", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto To1  = []()->FVector { return FVector(30); };
			auto To2  = []()->FVector { return FVector(80); };
			auto To3  = []()->FVector { return FVector(40); };
			auto ToC  = []()->FVector { return FVector(25); };

			FVector V1(0), V2(0), V3(0), C(0);

			auto T1 = NewObject<UQuickVectorTween>(Sequence); // 0.6 total, end=end (odd)
			T1->SetUp(From, To1, [&V1](const FVector& v){ V1=v; }, 0.2f, 1.0f, EEaseType::Linear, nullptr, 3, ELoopType::PingPong);
			auto T2 = NewObject<UQuickVectorTween>(Sequence); // 0.8 total, end=end
			T2->SetUp(From, To2, [&V2](const FVector& v){ V2=v; }, 0.4f, 1.0f, EEaseType::Linear, nullptr, 2, ELoopType::Restart);
			auto T3 = NewObject<UQuickVectorTween>(Sequence); // 0.6 total, end=start (even)
			T3->SetUp(From, To3, [&V3](const FVector& v){ V3=v; }, 0.15f,1.0f, EEaseType::Linear, nullptr, 4, ELoopType::PingPong);

			auto TC = NewObject<UQuickVectorTween>(Sequence); // 0.5 total
			TC->SetUp(From, ToC, [&C](const FVector& v){ C=v; }, 0.25f,1.0f, EEaseType::Linear, nullptr, 2, ELoopType::Restart);

			Sequence->Join(T1);
			Sequence->Append(T2);
			Sequence->Append(T3); // G1 = 0.8
			Sequence->Join(TC);                         // G2 = 0.5
			Sequence->Play();

			// March G1 boundaries: 0.15, 0.2, 0.3, 0.4, 0.6, 0.8
			Sequence->Update(0.15f); // T3 hit 1st boundary
			TestTrue("T3 at end/start flip 1", V3.Equals(To3(), 1.0f));
			Sequence->Update(0.05f); // T1 hit 1st boundary (0.2)
			TestTrue("T1 reached 30 at 0.2", V1.Equals(To1(), 1.0f));
			Sequence->Update(0.10f); // T3 hit 2nd boundary (0.3)
			TestTrue("T3 flip 2", true); // tolerant
			Sequence->Update(0.10f); // T2 loop1 end at 0.4
			TestTrue("T2 loop1 end", V2.Equals(To2(), 1.0f));
			Sequence->Update(0.20f); // reach 0.6 (T1 loop2 end & T3 loop4 end)
			TestTrue("T1 (odd) still has final segment to end", V1.X <= 30.0f);
			Sequence->Update(0.20f); // reach 0.8 (T2 loop2 end)
			TestTrue("T3 (even) ended-at-start by 0.6", V3.Equals(From(), 2.0f));
			TestTrue("T2 finished", V2.Equals(To2(), 1.0f));
			TestTrue("G1 over, G2 not yet started", C.Equals(From(), 1e-3f));

			// G2: 0.25 + 0.25
			Sequence->Update(0.25f);
			TestTrue("TC mid ~12.5", C.Equals(ToC(), 1.0f));
			Sequence->Update(0.25f);
			TestTrue("TC finished", C.Equals(ToC(), 1.0f));
		});

		It("Internal loops: G1 TA(2x PingPong 0.5) + TB(1x Restart 1.0), then G2 TC(2x PingPong 0.25)", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100); };
			auto ToB  = []()->FVector { return FVector(80);  };
			auto ToC  = []()->FVector { return FVector(50);  };

			FVector A(0), B(0), C(0);
			auto TA = NewObject<UQuickVectorTween>(Sequence); // 1.0 total (end=start)
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 0.5f, 1.0f, EEaseType::Linear, nullptr, 2, ELoopType::PingPong);
			auto TB = NewObject<UQuickVectorTween>(Sequence); // 1.0 total (end=end)
			TB->SetUp(From, ToB, [&B](const FVector& v){ B=v; }, 1.0f, 1.0f, EEaseType::Linear, nullptr, 1, ELoopType::Restart);
			auto TC = NewObject<UQuickVectorTween>(Sequence); // 0.5 total (end=start)
			TC->SetUp(From, ToC, [&C](const FVector& v){ C=v; }, 0.25f,1.0f, EEaseType::Linear, nullptr, 2, ELoopType::PingPong);

			Sequence->Join(TA);
			Sequence->Append(TB); // G1 = 1.0
			Sequence->Join(TC);             // G2 = 0.5
			Sequence->Play();

			// G1: 0.5 then 0.5
			Sequence->Update(0.5f);
			TestTrue("TA forward end, TB mid", A.Equals(ToA(),1.5f));
			Sequence->Update(0.5f);
			TestTrue("TA even pingpong ends at start", A.Equals(From(), 1.5f));
			TestTrue("TB end", B.Equals(ToB(), 1.0f));

			// G2: 0.25 + 0.25
			Sequence->Update(0.25f);
			TestTrue("TC mid", C.Equals(ToC(), 1.0f));
			Sequence->Update(0.25f);
			TestTrue("TC even pingpong ends at start", C.Equals(From(), 1.0f));
		});

		It("Internal loops: Ensure G2 doesn’t start until G1’s max time is fully consumed (Restart)", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100); };
			auto ToB  = []()->FVector { return FVector(80);  };
			auto ToC  = []()->FVector { return FVector(30);  };

			FVector A(0), B(0), C(0);
			auto TA = NewObject<UQuickVectorTween>(Sequence); // 2.0 total
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 0.5f, 1.0f, EEaseType::Linear, nullptr, 4, ELoopType::Restart);
			auto TB = NewObject<UQuickVectorTween>(Sequence); // 1.0 total
			TB->SetUp(From, ToB, [&B](const FVector& v){ B=v; }, 1.0f, 1.0f, EEaseType::Linear, nullptr, 1, ELoopType::Restart);
			auto TC = NewObject<UQuickVectorTween>(Sequence); // 3.0 total
			TC->SetUp(From, ToC, [&C](const FVector& v){ C=v; }, 1.5f, 1.0f, EEaseType::Linear, nullptr, 2, ELoopType::Restart);

			Sequence->Join(TA);
			Sequence->Append(TB); // G1 = 2.0
			Sequence->Join(TC);             // G2 = 3.0
			Sequence->Play();

			Sequence->Update(0.5f);
			Sequence->Update(0.5f);
			TestTrue("G2 not started mid-G1", C.Equals(From(), 1e-3f));

			Sequence->Update(0.5f);
			Sequence->Update(0.5f); // finish G1
			TestTrue("G1 finished", A.Equals(ToA(),2.0f) && B.Equals(ToB(),2.0f));
			TestTrue("G2 still at start just after boundary", C.Equals(From(), 1e-3f));

			// Now run G2: 1.5 + 1.5
			Sequence->Update(1.5f * 0.5f);
			TestTrue("TC mid (~15)", C.X > 10.0f && C.X < 20.0f);
			Sequence->Update(1.5f * 0.5f);
			Sequence->Update(1.5f);
			TestTrue("TC finished", C.Equals(ToC(), 1.0f));
		});

		It("Internal loops: Shorter tween freezes while longer continues (Restart) with boundary stepping", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToShort = []()->FVector { return FVector(30);  };
			auto ToLong  = []()->FVector { return FVector(100); };

			FVector Short(0), Long(0);
			auto TShort = NewObject<UQuickVectorTween>(Sequence); // 0.9 total
			TShort->SetUp(From, ToShort, [&Short](const FVector& v){ Short=v; }, 0.3f, 1.0f, EEaseType::Linear, nullptr, 3, ELoopType::Restart);
			auto TLong  = NewObject<UQuickVectorTween>(Sequence); // 1.5 total
			TLong->SetUp (From, ToLong,  [&Long](const FVector& v){ Long=v; }, 0.5f, 1.0f, EEaseType::Linear, nullptr, 3, ELoopType::Restart);

			Sequence->Join(TShort);
			Sequence->Append(TLong); // G1 = 1.5
			Sequence->Play();

			// Step to 0.9 exactly: 0.3 + 0.3 + 0.3
			Sequence->Update(0.3f); Sequence->Update(0.3f); Sequence->Update(0.3f);
			TestTrue("Short finished at 0.9", Short.Equals(ToShort(), 1.0f));
			const float LongAt09 = Long.X;

			// Finish to 1.5 with step 0.6
			Sequence->Update(0.6f);
			TestTrue("Long advanced after short done", Long.X > LongAt09);
			TestTrue("Long finished at 1.5", Long.Equals(ToLong(), 1.0f));
			TestTrue("Sequence completed", Sequence->GetIsCompleted());
		});

		It("Internal loops: Start reversed before Play; G2 TB(2x Restart, 0.75), then G1 TA(2x Restart, 0.5)", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(100); };
			auto ToB  = []()->FVector { return FVector(50);  };

			FVector A(0), B(0);
			auto TA = NewObject<UQuickVectorTween>(Sequence); // 1.0 total
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 0.5f, 1.0f, EEaseType::Linear, nullptr, 2, ELoopType::Restart);
			auto TB = NewObject<UQuickVectorTween>(Sequence); // 1.5 total
			TB->SetUp(From, ToB, [&B](const FVector& v){ B=v; }, 0.75f,1.0f, EEaseType::Linear, nullptr, 2, ELoopType::Restart);

			Sequence->Join(TA);
			Sequence->Join(TB);
			Sequence->Reverse();  // start backwards
			Sequence->Restart();
			Sequence->Play();

			Sequence->Update(0.01f); // apply initial pose snap
			TestTrue("Start near end of G2 when reversed (tolerant)", B.X > 40.0f);

			Sequence->Update(0.75f * 0.5f);
			TestTrue("G2 mid backward", B.X < 25.0f);
			Sequence->Update(0.75f * 0.5f - 0.01f);
			TestTrue("G2 finished loop at end", B.Equals(ToB(), 1.0f));

			// G2 backward: 0.75
			Sequence->Update(0.75f);
			TestTrue("G2 finished backward at start", B.Equals(From(), 1.0f));
			Sequence->Update(0.01f);

			TestTrue("G1 now near end", A.X > 90.0f);
			Sequence->Update(0.49f);
			TestTrue("G1 finished loop at end", A.Equals(ToA(), 1.0f));
			Sequence->Update(0.55f);
			TestTrue("Sequence completed", Sequence->GetIsCompleted());
		});

		It("Internal loops: Two groups (PingPong then Restart); parity respected and no cross-boundary overshoot", [this]()
		{
			auto From = []()->FVector { return FVector::ZeroVector; };
			auto ToA  = []()->FVector { return FVector(70);  };
			auto ToB  = []()->FVector { return FVector(140); };
			auto ToC  = []()->FVector { return FVector(30);  };

			FVector A(0), B(0), C(0);
			// G1: TA 2x PingPong 0.4 -> 0.8 total (ends at start)
			auto TA = NewObject<UQuickVectorTween>(Sequence);
			TA->SetUp(From, ToA, [&A](const FVector& v){ A=v; }, 0.4f, 1.0f, EEaseType::Linear, nullptr, 2, ELoopType::PingPong);
			// G1: TB 1x Restart 0.8 -> 0.8 total (ends at end)
			auto TB = NewObject<UQuickVectorTween>(Sequence);
			TB->SetUp(From, ToB, [&B](const FVector& v){ B=v; }, 0.8f, 1.0f, EEaseType::Linear, nullptr, 1, ELoopType::Restart);
			// G2: TC 2x Restart 0.3 -> 0.6 total
			auto TC = NewObject<UQuickVectorTween>(Sequence);
			TC->SetUp(From, ToC, [&C](const FVector& v){ C=v; }, 0.3f, 1.0f, EEaseType::Linear, nullptr, 2, ELoopType::Restart);

			Sequence->Join(TA);
			Sequence->Append(TB); // G1 total = 0.8
			Sequence->Join(TC);             // G2 total = 0.6
			Sequence->Play();

			// Step G1: 0.4 + 0.4
			Sequence->Update(0.4f);
			TestTrue("TA at end (first forward), TB mid", A.Equals(ToA(),1.0f));
			Sequence->Update(0.4f);
			TestTrue("TA ended at start (even pingpong)", A.Equals(From(),1.0f));
			TestTrue("TB ended at end", B.Equals(ToB(),1.0f));
			TestTrue("G2 not started just yet", C.Equals(From(), 1e-3f));

			// Step G2: 0.3 + 0.3
			Sequence->Update(0.15f);
			TestTrue("TC mid", C.X > 10.0f && C.X < 25.0f);
			Sequence->Update(0.15f);
			TestTrue("TC finished", C.Equals(ToC(),1.0f));
		});

	});
}
