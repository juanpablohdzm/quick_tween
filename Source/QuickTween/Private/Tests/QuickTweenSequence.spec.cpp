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
			Sequence->AddToRoot();
			Sequence->SetLoops(1);
			Sequence->SetLoopType(ELoopType::Restart);

			Listener = NewObject<UQuickTweenSequenceTestListener>(GetTransientPackage());
			Listener->AddToRoot();

			Sequence->OnStart.AddDynamic(Listener, &UQuickTweenSequenceTestListener::HandleStart);
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
			// Append a tween but don't press Play()
			FVector ACurrent(0,0,0);
			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->Initialize(FVector::ZeroVector, FVector(100,100,100),
				[&ACurrent](const FVector& V){ ACurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA);

			Sequence->Update(0.5f);

			TestFalse("Sequence should not be playing", Sequence->GetIsPlaying());
			TestEqual("Elapsed time remains zero", Sequence->GetElapsedTime(), 0.0f);
			TestTrue("Joined tween should not have updated", ACurrent.Equals(FVector::ZeroVector, 1e-3f));
		});

		It("Should join tweens and run sequentially", [this]()
		{
			FVector ACurrent(0,0,0), BCurrent(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->Initialize(FVector::ZeroVector, FVector(100,100,100),
				[&ACurrent](const FVector& V){ ACurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			auto TweenB = NewObject<UQuickVectorTween>(Sequence);
			TweenB->Initialize(FVector::ZeroVector, FVector(50,50,50),
				[&BCurrent](const FVector& V){ BCurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Join(TweenA)->Join(TweenB);
			Sequence->SetId("Seq_Append");

			Sequence->Play();

			// Halfway through first tween
			Sequence->Update(0.5f);
			TestTrue("First tween ~halfway", ACurrent.Equals(FVector(50,50,50), 10.0f));
			TestTrue("Second tween not started yet", BCurrent.Equals(FVector::ZeroVector, 1e-3f));

			// Finish first + start second a bit
			Sequence->Update(0.6f); // total 1.1s
			TestTrue("First tween finished", ACurrent.Equals(FVector(100,100,100), 1.0f));
			TestTrue("Second tween started (~0.1)", BCurrent.Equals(FVector(5,5,5), 2.0f));

			// Complete sequence
			Sequence->Update(2.0f);
			TestTrue("Sequence completed", Sequence->GetIsCompleted());
			TestTrue("Progress clamped to 1", FMath::IsNearlyEqual(Sequence->GetElapsedTime(), Sequence->GetDuration(), 1e-3f));
			TestTrue("Second tween finished", BCurrent.Equals(FVector(50,50,50), 1.0f));

			TestEqual("OnStart called once", Listener->StartCalls, 1);
			TestTrue("OnUpdate called at least once", Listener->UpdateCalls > 0);
			TestEqual("OnComplete called once", Listener->CompleteCalls, 1);
			TestEqual("Sequence Id matches", Sequence->GetId(), FString("Seq_Append"));
		});

		It("Should append tweens and run them in parallel, taking the max duration", [this]()
		{
			FVector ACurrent(0,0,0), BCurrent(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->Initialize(FVector::ZeroVector, FVector(100,100,100),
				[&ACurrent](const FVector& V){ ACurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			auto TweenB = NewObject<UQuickVectorTween>(Sequence);
			TweenB->Initialize(FVector::ZeroVector, FVector(200,200,200),
				[&BCurrent](const FVector& V){ BCurrent = V; }, 2.0f, 1.0f, EEaseType::Linear);

			// Start a group with A, then append B to the same group
			Sequence->Join(TweenA)->Append(TweenB);

			TestTrue("Duration should be the max of joined tweens (~2s)", FMath::IsNearlyEqual(Sequence->GetDuration(), 2.0f, 1e-3f));

			Sequence->Play();

			Sequence->Update(0.5f);
			TestTrue("A ~50%", ACurrent.Equals(FVector(50,50,50), 10.0f));
			TestTrue("B ~25%", BCurrent.Equals(FVector(50,50,50), 10.0f));

			Sequence->Update(0.5f); // t = 1.0
			TestTrue("A finished at 1s", ACurrent.Equals(FVector(100,100,100), 1.0f));
			TestTrue("B ~50% at 1s", BCurrent.Equals(FVector(100,100,100), 10.0f));
			TestFalse("Sequence not completed until the longest tween ends", Sequence->GetIsCompleted());

			Sequence->Update(1.0f); // t = 2.0
			TestTrue("B finished at 2s", BCurrent.Equals(FVector(200,200,200), 1.0f));
			TestTrue("Sequence completed at max duration", Sequence->GetIsCompleted());
		});

		It("Should support Pause and TogglePause", [this]()
		{
			FVector Current(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->Initialize(FVector::ZeroVector, FVector(100,100,100),
				[&Current](const FVector& V){ Current = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA);
			Sequence->Play();

			Sequence->Update(0.5f);
			TestTrue("~50% before pause", Current.Equals(FVector(50,50,50), 10.0f));

			Sequence->Pause();
			const FVector ValueAtPause = Current;
			Sequence->Update(0.5f);
			TestTrue("No change while paused", Current.Equals(ValueAtPause, 1e-3f));

			Sequence->TogglePause(); // resume
			Sequence->Update(0.5f);
			TestTrue("Finished after resume", Current.Equals(FVector(100,100,100), 1.0f));
			TestTrue("Sequence completed", Sequence->GetIsCompleted());
		});

		It("Should Complete immediately", [this]()
		{
			FVector ACurrent(0,0,0), BCurrent(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->Initialize(FVector::ZeroVector, FVector(100,100,100),
				[&ACurrent](const FVector& V){ ACurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			auto TweenB = NewObject<UQuickVectorTween>(Sequence);
			TweenB->Initialize(FVector::ZeroVector, FVector(50,50,50),
				[&BCurrent](const FVector& V){ BCurrent = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA)->Append(TweenB);
			Sequence->Play();

			Sequence->Update(0.3f);
			Sequence->Complete();

			TestTrue("Sequence is completed", Sequence->GetIsCompleted());
			TestTrue("Progress equals duration", FMath::IsNearlyEqual(Sequence->GetElapsedTime(), Sequence->GetDuration(), 1e-3f));
			TestTrue("First tween at end", ACurrent.Equals(FVector(100,100,100), 1.0f));
			TestTrue("Second tween at end", BCurrent.Equals(FVector(50,50,50), 1.0f));
			TestEqual("OnComplete called once", Listener->CompleteCalls, 1);
		});

		It("Should Restart from the beginning", [this]()
		{
			FVector Current(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->Initialize(FVector::ZeroVector, FVector(100,100,100),
				[&Current](const FVector& V){ Current = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA);
			Sequence->Play();

			Sequence->Update(0.8f);
			TestTrue("Near end before restart", Current.Equals(FVector(80,80,80), 10.0f));

			Sequence->Restart();
			TestEqual("Elapsed time reset", Sequence->GetElapsedTime(), 0.0f);
			TestTrue("Value reset", Current.Equals(FVector::ZeroVector, 1.0f));
			TestFalse("Not completed after restart", Sequence->GetIsCompleted());

			Sequence->Update(1.0f);
			TestTrue("Finished after restart", Current.Equals(FVector(100,100,100), 1.0f));
		});

		It("Should loop with Restart correctly", [this]()
		{
			FVector Current(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->Initialize(FVector::ZeroVector, FVector(100,100,100),
				[&Current](const FVector& V){ Current = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA)->SetLoops(2)->SetLoopType(ELoopType::Restart);

			Sequence->Play();
			TestEqual("CurrentLoop starts at 1", Sequence->GetCurrentLoop(), 1.0f);

			Sequence->Update(0.99f);
			TestFalse("Not completed yet (end of loop 1)", Sequence->GetIsCompleted());

			Sequence->Update(0.02f); // cross into loop 2
			TestTrue("Value reset at loop restart", Current.Equals(FVector::ZeroVector, 10.0f));
			TestEqual("CurrentLoop moved to 2", Sequence->GetCurrentLoop(), 2.0f);

			Sequence->Update(1.0f);
			TestTrue("Sequence completed after 2 loops", Sequence->GetIsCompleted());
			TestTrue("Final value at target", Current.Equals(FVector(100,100,100), 1.0f));
		});

		It("Should loop with PingPong correctly", [this]()
		{
			FVector Current(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->Initialize(FVector::ZeroVector, FVector(100,100,100),
				[&Current](const FVector& V){ Current = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA)->SetLoops(2)->SetLoopType(ELoopType::PingPong);

			Sequence->Play();

			// Forward
			Sequence->Update(0.5f);
			TestTrue("~halfway forward", Current.Equals(FVector(50,50,50), 10.0f));
			Sequence->Update(0.5f);
			TestTrue("Reached end of forward", Current.Equals(FVector(100,100,100), 1.0f));
			TestFalse("Not completed after forward half", Sequence->GetIsCompleted());

			// Backward
			Sequence->Update(0.5f);
			TestTrue("~halfway backward", Current.Equals(FVector(50,50,50), 10.0f));

			Sequence->Update(0.5f);
			TestTrue("Returned to start after ping-pong", Current.Equals(FVector::ZeroVector, 1.0f));
			TestTrue("Sequence completed after ping-pong", Sequence->GetIsCompleted());
		});

		It("Should KillSequence and mark as pending kill", [this]()
		{
			FVector Current(0,0,0);

			auto TweenA = NewObject<UQuickVectorTween>(Sequence);
			TweenA->Initialize(FVector::ZeroVector, FVector(100,100,100),
				[&Current](const FVector& V){ Current = V; }, 1.0f, 1.0f, EEaseType::Linear);

			Sequence->Append(TweenA);
			Sequence->Play();

			Sequence->Update(0.2f);
			Sequence->KillSequence();

			TestTrue("Sequence pending kill", Sequence->GetIsPendingKill());
			TestEqual("OnKilled fired once", Listener->KilledCalls, 1);

			// Further updates shouldn't do anything meaningful
			const FVector ValueAtKill = Current;
			Sequence->Update(0.5f);
			TestTrue("No updates after kill", Current.Equals(ValueAtKill, 1e-3f));
		});

		It("Should report num tweens and allow GetTween access", [this]()
		{
			TArray<UQuickVectorTween*> Tweens;

			for (int i = 0; i < 3; ++i)
			{
				auto T = NewObject<UQuickVectorTween>(Sequence);
				FVector To( (i+1) * 10 );
				T->Initialize(FVector::ZeroVector, FVector(To.X, To.X, To.X),
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
	});
}
