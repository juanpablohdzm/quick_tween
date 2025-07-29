#include "Misc/AutomationTest.h"
#include "Tweens/QuickVectorTween.h"

BEGIN_DEFINE_SPEC(AutomationSpec, "Gameplay.QuickTween", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
UQuickVectorTween* vectorTween;
END_DEFINE_SPEC(AutomationSpec)

void AutomationSpec::Define()
{
	Describe("QuickTween Automation Tests Vector", [this]()
	{
		BeforeEach([this]()
		{
			vectorTween = NewObject<UQuickVectorTween>();
		});

		It("Should initialize and update a vector tween", [this]()
		{
			FVector from(0, 0, 0);
			FVector to(100, 100, 100);
			FVector currentValue;
			vectorTween->Initialize(from, to, [this, &currentValue](const FVector& value) mutable
			{
				currentValue = value;
			}, 1.0f);

			vectorTween->Play();
			// Simulate an update
			vectorTween->Update(0.5f); // Simulate 60 FPS

			// Check if the tween has been updated correctly
			TestTrue("Tween should have progressed", vectorTween->GetProgress() > 0.0f);
			TestTrue("Current value should be close to halfway", FVector::Dist(currentValue, FVector(50, 50, 50)) < KINDA_SMALL_NUMBER);
		});


		It("Should initialize and not update a vector tween if not playing", [this]()
		{
			FVector from(0, 0, 0);
			FVector to(100, 100, 100);
			vectorTween->Initialize(from, to, [this](const FVector& value)
			{

			}, 1.0f);

			// Simulate an update
			for (int i = 0; i < 30; ++i)
			{
				vectorTween->Update(1.0f / 60.0f); // Simulate 60 FPS
			}

			// Check if the tween has been updated correctly
			TestFalse("Tween should have not progressed", vectorTween->GetProgress() > 0.0f);
		});

		It("Should handle simple ease from beginning to end", [this]()
		{
			FVector from(0, 0, 0);
			FVector to(100, 100, 100);
			FVector currentValue;
			vectorTween->Initialize(from, to, [this, &currentValue](const FVector& value) mutable
			{
				// This is where you would normally set the value on a target
				currentValue = value;
			}, 1.0f, 1.0f, EEaseType::Linear);

			vectorTween->Play();
			for (int i = 0; i < 70; ++i)
			{
				vectorTween->Update(1.0f / 60.0f); // Simulate 60 FPS
			}

			TestTrue("Tween should have reached the end", vectorTween->GetIsCompleted());
			TestNearlyEqual("Final value should be close to target", currentValue, to);
		});

		It("Should not update after end", [this]()
		{
			FVector from(0, 0, 0);
			FVector to(100, 100, 100);
			FVector currentValue;
			vectorTween->Initialize(from, to, [this, &currentValue](const FVector& value) mutable
			{
				// This is where you would normally set the value on a target
				currentValue = value;
			}, 1.0f, 1.0f, EEaseType::Linear);

			vectorTween->Play();
			for (int i = 0; i < 70; ++i)
			{
				vectorTween->Update(1.0f / 60.0f); // Simulate 60 FPS
			}

			TestTrue("Tween should have reached the end", vectorTween->GetIsCompleted());
			TestTrue("Final value should be close to target", FVector::Dist(currentValue, to) < KINDA_SMALL_NUMBER);

			for (int i = 0; i < 60; ++i)
			{
				vectorTween->Update(1.0f / 60.0f);
			}

			TestFalse("Tween should not update after completion", vectorTween->GetProgress() >= 1.0f);
			TestTrue("Final value should still be close to target", FVector::Dist(currentValue, to) < KINDA_SMALL_NUMBER);
		});

		It("Should handle slow time scaling correctly", [this]()
		{
			FVector from(0, 0, 0);
			FVector to(100, 100, 100);
			FVector currentValue;
			vectorTween->Initialize(from, to, [this, &currentValue](const FVector& value) mutable
			{
				currentValue = value;
			}, 1.0f, 0.5f, EEaseType::Linear);

			vectorTween->Play();
			for (int i = 0; i < 60; ++i)
			{
				vectorTween->Update(1.0f / 60.0f); // Simulate 60 FPS
			}

			TestTrue("Final value should be close to middle target", FVector::Dist(currentValue, to / 2) < KINDA_SMALL_NUMBER);
			TestFalse("Tween should not be completed yet", vectorTween->GetIsCompleted());

			for (int i = 0; i < 90; ++i)
			{
				vectorTween->Update(1.0f / 60.0f); // Simulate 60 FPS
			}

			TestTrue("Tween should have reached the end after time scaling", vectorTween->GetIsCompleted());
		});

		It("Should handle fast time scaling correctly", [this]()
		{
			FVector from(0, 0, 0);
			FVector to(100, 100, 100);
			FVector currentValue;
			vectorTween->Initialize(from, to, [this, &currentValue](const FVector& value) mutable
			{
				currentValue = value;
			}, 1.0f, 2.0f, EEaseType::Linear);

			vectorTween->Play();
			for (int i = 0; i < 40; ++i)
			{
				vectorTween->Update(1.0f / 60.0f); // Simulate 60 FPS
			}

			TestNearlyEqual("Final value should be close to final target", currentValue, to);
			TestTrue("Tween should be completed", vectorTween->GetIsCompleted());

		});

		AfterEach([this]()
		{
			// Clean up the vectorTween object
			if (vectorTween)
			{
				vectorTween->ConditionalBeginDestroy();
				vectorTween = nullptr;
			}
		});
	});
}
