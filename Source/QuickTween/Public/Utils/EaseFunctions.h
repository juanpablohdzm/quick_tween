// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

enum class QUICKTWEEN_API EEasePath : uint8
{
    Default,
    Shortest,
    Longest
};

template <typename T>
struct TEaseLerp
{
    static FORCEINLINE T Lerp(const T& A, const T& B, float Alpha, EEasePath /*Path*/)
    {
        return FMath::Lerp(A, B, Alpha);
    }
};

// --- Specialization for FRotator: supports Shortest/Longest/Default ---
template <>
struct TEaseLerp<FRotator>
{
    static FORCEINLINE FRotator Lerp(const FRotator& A, const FRotator& B, float Alpha, EEasePath Path)
    {
        Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
        if (Alpha <= 0.f) return A;
        if (Alpha >= 1.f) return B;

        const FQuat QA = A.Quaternion();
        FQuat QB = B.Quaternion();

        float Dot = QA | QB;

        if (Dot > 1.f - UE_KINDA_SMALL_NUMBER)
        {
            // Quats are nearly the same: use a cheap lerp and normalize.
            FQuat R = FQuat::FastLerp(QA, QB, Alpha);
            R.Normalize();
            return R.Rotator();
        }

        if (Path == EEasePath::Longest)
        {
            // Ensure we traverse the LONG arc: we want a NEGATIVE dot for FullPath.
            if (Dot > 0.f) { QB = -QB; }
            const FQuat R = FQuat::SlerpFullPath(QA, QB, Alpha);
            return R.Rotator();
        }

        // Slerp picks the shortest arc internally (flips if dot < 0).
        const FQuat R = FQuat::Slerp(QA, QB, Alpha);
        return R.Rotator();

    }
};

/**
 * @class FEaseFunctions
 * @brief Provides a collection of static easing functions for interpolation.
 *
 * Template class for various easing functions, supporting types compatible with TEaseLerp<T>::Lerp.
 * Easing functions are used to create smooth transitions and animations.
 *
 * @tparam T Type to interpolate (e.g., float, FVector).
 */
template <typename T>
class QUICKTWEEN_API FEaseFunctions
{
public:
    /**
     * \brief Interpolates between Start and End using the specified easing type.
     *
     * \param Start The starting value.
     * \param End The ending value.
     * \param Alpha The interpolation factor (0.0 to 1.0).
     * \param EaseType The type of easing to use.
     * \return Interpolated value.
     */
    static T Ease(T Start, T End, float Alpha, EEaseType EaseType, EEasePath Path = EEasePath::Default)
    {
        switch (EaseType)
        {
            case EEaseType::Linear:
                return TEaseLerp<T>::Lerp(Start, End, Alpha, Path);
            case EEaseType::InSine:
                return EaseInSine(Start, End, Alpha, Path);
            case EEaseType::OutSine:
                return EaseOutSine(Start, End, Alpha, Path);
            case EEaseType::InOutSine:
                return EaseInOutSine(Start, End, Alpha, Path);
            case EEaseType::InQuad:
                return EaseInQuad(Start, End, Alpha, Path);
            case EEaseType::OutQuad:
                return EaseOutQuad(Start, End, Alpha, Path);
            case EEaseType::InOutQuad:
                return EaseInOutQuad(Start, End, Alpha, Path);
            case EEaseType::InCubic:
                return EaseInCubic(Start, End, Alpha, Path);
            case EEaseType::OutCubic:
                return EaseOutCubic(Start, End, Alpha, Path);
            case EEaseType::InOutCubic:
                return EaseInOutCubic(Start, End, Alpha, Path);
            case EEaseType::InQuart:
                return EaseInQuart(Start, End, Alpha, Path);
            case EEaseType::OutQuart:
                return EaseOutQuart(Start, End, Alpha, Path);
            case EEaseType::InOutQuart:
                return EaseInOutQuart(Start, End, Alpha, Path);
            case EEaseType::InQuint:
                return EaseInQuint(Start, End, Alpha, Path);
            case EEaseType::OutQuint:
                return EaseOutQuint(Start, End, Alpha, Path);
            case EEaseType::InOutQuint:
                return EaseInOutQuint(Start, End, Alpha, Path);
            case EEaseType::InExpo:
                return EaseInExpo(Start, End, Alpha, Path);
            case EEaseType::OutExpo:
                return EaseOutExpo(Start, End, Alpha, Path);
            case EEaseType::InOutExpo:
                return EaseInOutExpo(Start, End, Alpha, Path);
            default: // Circ
                return TEaseLerp<T>::Lerp(Start, End,
                                   1.f - FMath::Sqrt(1.f - FMath::Pow(Alpha * 2.f - 1.f, 2.f)), Path);
        }
    }

    static T Ease(T Start, T End, float Alpha, const UCurveFloat* EaseCurve, EEasePath Path = EEasePath::Default)
    {
        ensureAlways(EaseCurve);
        return TEaseLerp<T>::Lerp(Start, End, EaseCurve->GetFloatValue(Alpha), Path);
    }
    // ---------------------
    // Sine
    // ---------------------

    /**
     * \brief Sine ease-in interpolation.
     */
    static T EaseInSine(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, 1.f - FMath::Cos(Alpha * PI / 2.f), Path);
    }

    /**
     * \brief Sine ease-out interpolation.
     */
    static T EaseOutSine(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, FMath::Sin(Alpha * PI / 2.f), Path);
    }

    /**
     * \brief Sine ease-in-out interpolation.
     */
    static T EaseInOutSine(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, 0.5f * (1.f - FMath::Cos(Alpha * PI)), Path);
    }

    // ---------------------
    // Quadratic
    // ---------------------

    /**
     * \brief Quadratic ease-in interpolation.
     */
    static T EaseInQuad(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, Alpha * Alpha, Path);
    }

    /**
     * \brief Quadratic ease-out interpolation.
     */
    static T EaseOutQuad(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, Alpha * (2.f - Alpha), Path);
    }

    /**
     * \brief Quadratic ease-in-out interpolation.
     */
    static T EaseInOutQuad(T Start, T End, float Alpha, EEasePath Path)
    {
        float Result = (Alpha < 0.5f)
            ? 2.f * Alpha * Alpha
            : -1.f + (4.f - 2.f * Alpha) * Alpha;
        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }

    // ---------------------
    // Cubic
    // ---------------------

    /**
     * \brief Cubic ease-in interpolation.
     */
    static T EaseInCubic(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, Alpha * Alpha * Alpha, Path);
    }

    /**
     * \brief Cubic ease-out interpolation.
     */
    static T EaseOutCubic(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, 1.f - FMath::Pow(1.f - Alpha, 3), Path);
    }

    /**
     * \brief Cubic ease-in-out interpolation.
     */
    static T EaseInOutCubic(T Start, T End, float Alpha, EEasePath Path)
    {
        float Result = (Alpha < 0.5f)
            ? 4.f * Alpha * Alpha * Alpha
            : 1.f - FMath::Pow(-2.f * Alpha + 2.f, 3.f) / 2.f;
        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }

    // ---------------------
    // Quartic
    // ---------------------

    /**
     * \brief Quartic ease-in interpolation.
     */
    static T EaseInQuart(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, FMath::Pow(Alpha, 4.f), Path);
    }

    /**
     * \brief Quartic ease-out interpolation.
     */
    static T EaseOutQuart(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, 1.f - FMath::Pow(1.f - Alpha, 4.f), Path);
    }

    /**
     * \brief Quartic ease-in-out interpolation.
     */
    static T EaseInOutQuart(T Start, T End, float Alpha, EEasePath Path)
    {
        float Result = (Alpha < 0.5f)
            ? 8.f * FMath::Pow(Alpha, 4.f)
            : 1.f - FMath::Pow(-2.f * Alpha + 2.f, 4.f) / 2.f;
        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }

    // ---------------------
    // Quintic
    // ---------------------

    /**
     * \brief Quintic ease-in interpolation.
     */
    static T EaseInQuint(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, FMath::Pow(Alpha, 5.f), Path);
    }

    /**
     * \brief Quintic ease-out interpolation.
     */
    static T EaseOutQuint(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, 1.f - FMath::Pow(1.f - Alpha, 5.f), Path);
    }

    /**
     * \brief Quintic ease-in-out interpolation.
     */
    static T EaseInOutQuint(T Start, T End, float Alpha, EEasePath Path)
    {
        float Result = (Alpha < 0.5f)
            ? 16.f * FMath::Pow(Alpha, 5.f)
            : 1.f - FMath::Pow(-2.f * Alpha + 2.f, 5.f) / 2.f;
        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }

    // ---------------------
    // Exponential
    // ---------------------

    /**
     * \brief Exponential ease-in interpolation.
     */
    static T EaseInExpo(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, (Alpha == 0.f) ? 0.f : FMath::Pow(2.f, 10.f * Alpha - 10.f), Path);
    }

    /**
     * \brief Exponential ease-out interpolation.
     */
    static T EaseOutExpo(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, (Alpha == 1.f) ? 1.f : 1.f - FMath::Pow(2.f, -10.f * Alpha), Path);
    }

    /**
     * \brief Exponential ease-in-out interpolation.
     */
    static T EaseInOutExpo(T Start, T End, float Alpha, EEasePath Path)
    {
        if (Alpha == 0.f) return Start;
        if (Alpha == 1.f) return End;

        float Result = (Alpha < 0.5f)
            ? FMath::Pow(2.f, 20.f * Alpha - 10.f) / 2.f
            : (2.f - FMath::Pow(2.f, -20.f * Alpha + 10.f)) / 2.f;

        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }

    // ---------------------
    // Circular
    // ---------------------

    /**
     * \brief Circular ease-in interpolation.
     */
    static T EaseInCirc(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, 1.f - FMath::Sqrt(1.f - Alpha * Alpha), Path);
    }

    /**
     * \brief Circular ease-out interpolation.
     */
    static T EaseOutCirc(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, FMath::Sqrt(1.f - FMath::Pow(Alpha - 1.f, 2.f)), Path);
    }

    /**
     * \brief Circular ease-in-out interpolation.
     */
    static T EaseInOutCirc(T Start, T End, float Alpha, EEasePath Path)
    {
        float Result = (Alpha < 0.5f)
            ? (1.f - FMath::Sqrt(1.f - 4.f * Alpha * Alpha)) / 2.f
            : (FMath::Sqrt(1.f - FMath::Pow(-2.f * Alpha + 2.f, 2.f)) + 1.f) / 2.f;

        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }

    // ---------------------
    // Back
    // ---------------------

    /**
     * \brief Back ease-in interpolation.
     */
    static T EaseInBack(T Start, T End, float Alpha, EEasePath Path)
    {
        constexpr float C1 = 1.70158f;
        constexpr float C3 = C1 + 1.f;
        return TEaseLerp<T>::Lerp(Start, End, Alpha * Alpha * (C3 * Alpha - C1), Path);
    }

    /**
     * \brief Back ease-out interpolation.
     */
    static T EaseOutBack(T Start, T End, float Alpha, EEasePath Path)
    {
        constexpr float C1 = 1.70158f;
        constexpr float C3 = C1 + 1.f;
        float Inv = 1.f - Alpha;
        return TEaseLerp<T>::Lerp(Start, End, 1.f - Inv * Inv * (C3 * Inv - C1), Path);
    }

    /**
     * \brief Back ease-in-out interpolation.
     */
    static T EaseInOutBack(T Start, T End, float Alpha, EEasePath Path)
    {
        constexpr float C1 = 1.70158f;
        constexpr float C2 = C1 * 1.525f;

        float Result = (Alpha < 0.5f)
            ? (FMath::Pow(2.f * Alpha, 2.f) * ((C2 + 1.f) * 2.f * Alpha - C2)) / 2.f
            : (FMath::Pow(2.f * Alpha - 2.f, 2.f) * ((C2 + 1.f) * (Alpha * 2.f - 2.f) + C2) + 2.f) / 2.f;

        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }

    // ---------------------
    // Elastic
    // ---------------------

    /**
     * \brief Elastic ease-in interpolation.
     */
    static T EaseInElastic(T Start, T End, float Alpha, EEasePath Path)
    {
        if (Alpha == 0.f) return Start;
        if (Alpha == 1.f) return End;

        constexpr float C4 = (2.f * PI) / 3.f;
        float Result = -FMath::Pow(2.f, 10.f * Alpha - 10.f) * FMath::Sin((Alpha * 10.f - 10.75f) * C4);

        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }

    /**
     * \brief Elastic ease-out interpolation.
     */
    static T EaseOutElastic(T Start, T End, float Alpha, EEasePath Path)
    {
        if (Alpha == 0.f) return Start;
        if (Alpha == 1.f) return End;

        constexpr float C4 = (2.f * PI) / 3.f;
        float Result = FMath::Pow(2.f, -10.f * Alpha) * FMath::Sin((Alpha * 10.f - 0.75f) * C4) + 1.f;

        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }

    /**
     * \brief Elastic ease-in-out interpolation.
     */
    static T EaseInOutElastic(T Start, T End, float Alpha, EEasePath Path)
    {
        if (Alpha == 0.f) return Start;
        if (Alpha == 1.f) return End;

        constexpr float C5 = (2.f * PI) / 4.5f;

        float Result = (Alpha < 0.5f)
            ? -(FMath::Pow(2.f, 20.f * Alpha - 10.f) * FMath::Sin((20.f * Alpha - 11.125f) * C5)) / 2.f
            : (FMath::Pow(2.f, -20.f * Alpha + 10.f) * FMath::Sin((20.f * Alpha - 11.125f) * C5)) / 2.f + 1.f;

        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }

    // ---------------------
    // Bounce
    // ---------------------

    /**
     * \brief Bounce ease-out interpolation.
     */
    static T EaseOutBounce(T Start, T End, float Alpha, EEasePath Path)
    {
        constexpr float N1 = 7.5625f;
        constexpr float D1 = 2.75f;

        float Result;

        if (Alpha < 1.f / D1)
        {
            Result = N1 * Alpha * Alpha;
        }
        else if (Alpha < 2.f / D1)
        {
            Alpha -= 1.5f / D1;
            Result = N1 * Alpha * Alpha + 0.75f;
        }
        else if (Alpha < 2.5f / D1)
        {
            Alpha -= 2.25f / D1;
            Result = N1 * Alpha * Alpha + 0.9375f;
        }
        else
        {
            Alpha -= 2.625f / D1;
            Result = N1 * Alpha * Alpha + 0.984375f;
        }

        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }

    /**
     * \brief Bounce ease-in interpolation.
     */
    static T EaseInBounce(T Start, T End, float Alpha, EEasePath Path)
    {
        return TEaseLerp<T>::Lerp(Start, End, 1.f - EaseOutBounce(0.f, 1.f, 1.f - Alpha), Path);
    }

    /**
     * \brief Bounce ease-in-out interpolation.
     */
    static T EaseInOutBounce(T Start, T End, float Alpha, EEasePath Path)
    {
        float Result = (Alpha < 0.5f)
            ? (1.f - EaseOutBounce(0.f, 1.f, 1.f - 2.f * Alpha)) / 2.f
            : (1.f + EaseOutBounce(0.f, 1.f, 2.f * Alpha - 1.f)) / 2.f;

        return TEaseLerp<T>::Lerp(Start, End, Result, Path);
    }
};
