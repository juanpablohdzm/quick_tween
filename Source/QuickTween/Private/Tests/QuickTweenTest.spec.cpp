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
			TestNearlyEqual("Current value should be close to halfway",currentValue, FVector(50, 50, 50), 10.0f);
		});


		It("Should initialize and not update a vector tween if not playing", [this]()
		{
			FVector from(0, 0, 0);
			FVector to(100, 100, 100);
			vectorTween->Initialize(from, to, [this](const FVector& value)
			{

			}, 1.0f);

			// Simulate an update
			vectorTween->Update(0.5f);

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
			for (int i = 0; i < 100; ++i)
			{
				vectorTween->Update(0.1f); // Simulate 60 FPS
			}

			TestTrue("Tween should have reached the end", vectorTween->GetIsCompleted());
			TestNearlyEqual("Final value should be close to target", currentValue, to, 10.0f);
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
			for (int i = 0; i < 100; ++i)
			{
				vectorTween->Update(0.1f); // Simulate 60 FPS
			}

			TestTrue("Tween should have reached the end", vectorTween->GetIsCompleted());
			TestNearlyEqual("Final value should be close to target", currentValue, to, 10.0f);

			for (int i = 0; i < 60; ++i)
			{
				vectorTween->Update(0.1f);
			}

			TestFalse("Tween should not update after completion", vectorTween->GetProgress() >= 1.0f);
			TestNearlyEqual("Final value should still be close to target", currentValue, to, 10.0f);
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
			vectorTween->Update(1.0f);

			TestNearlyEqual("Final value should be close to middle target", currentValue, to / 2.0f, 2.0f);
			TestFalse("Tween should not be completed yet", vectorTween->GetIsCompleted());

			vectorTween->Update(1.0f);

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

			for (int i = 0; i < 100; ++i)
			{
				vectorTween->Update(0.05f);
			}

			TestNearlyEqual("Final value should be close to final target", currentValue, to, 10.0f);
			TestTrue("Tween should be completed", vectorTween->GetIsCompleted());

		});


		It("Should handle looping restart correctly", [this]()
		{
			FVector from(0, 0, 0);
			FVector to(100, 100, 100);
			FVector currentValue;
			int8_t loopsAmount = 2;
			vectorTween->Initialize(from, to, [this, &currentValue](const FVector& value) mutable
			{
				currentValue = value;
			},
			1.0f,
			1.0f,
			EEaseType::Linear,
			nullptr,
			loopsAmount,
			ELoopType::Restart);

			// First loop
			vectorTween->Play();
			TestNearlyEqual("Value should be close to initial target at start", currentValue, from, 10.0f);
			vectorTween->Update(0.5f);
			TestNearlyEqual("Value should be close to halfway target", currentValue, FVector(50, 50, 50), 10.0f);
			vectorTween->Update(0.49f);
			TestNearlyEqual("Value should be close to final target after first loop", currentValue, to, 10.0f);
			TestFalse("Tween should not be completed after first loop", vectorTween->GetIsCompleted());

			vectorTween->Update(0.02f);
			// Second loop starts
			TestNearlyEqual("Value should be close to initial target", currentValue, from, 10.0f);
			vectorTween->Update(1.2f);
			TestNearlyEqual("Value should be at target after second loop", currentValue, to, 10.0f);

			TestTrue("Tween should be completed after all loops", vectorTween->GetIsCompleted());
		});

		It("Should handle looping ping-pong correctly", [this]()
		{
			FVector from(0, 0, 0);
			FVector to(100, 100, 100);
			FVector currentValue;
			int8_t loopsAmount = 2;
			vectorTween->Initialize(from, to, [this, &currentValue](const FVector& value) mutable
			{
				UE_LOG(LogTemp, Log, TEXT("Current Value: %s"), *value.ToString());
				currentValue = value;
			},
			1.0f,
			1.0f,
			EEaseType::Linear,
			nullptr,
			loopsAmount,
			ELoopType::PingPong);

			// First forward loop
			vectorTween->Play();
			TestNearlyEqual("Value should be close to initial target at start", currentValue, from, 10.0f);
			vectorTween->Update(0.5f);
			TestNearlyEqual("Value should be close to halfway target", currentValue, FVector(50, 50, 50), 10.0f);
			vectorTween->Update(0.49f);
			TestNearlyEqual("Value should be close to final target after first forward loop", currentValue, to, 10.0f);
			TestFalse("Tween should not be completed after first forward loop", vectorTween->GetIsCompleted());

			// First backward loop
			vectorTween->Update(0.02f);
			TestNearlyEqual("Value should be close to final target at start of backward loop", currentValue, to, 10.0f);
			vectorTween->Update(0.48f);
			TestNearlyEqual("Value should be close to halfway target on backward", currentValue, FVector(50, 50, 50), 10.0f);
			vectorTween->Update(1.0f);
			TestNearlyEqual("Value should be close to initial target after first backward loop", currentValue, from, 10.0f);

			TestTrue("Tween should be completed after all ping-pong loops", vectorTween->GetIsCompleted());
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
