// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

//TODO: Fix for Rotators

/**
 * \class FEaseFunctions
 * \brief Provides a collection of static easing functions for interpolation.
 *
 * Template class for various easing functions, supporting types compatible with FMath::Lerp.
 * Easing functions are used to create smooth transitions and animations.
 *
 * \tparam T Type to interpolate (e.g., float, FVector).
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
    static T Ease(T Start, T End, float Alpha, EEaseType EaseType)
    {
        switch (EaseType)
        {
            case EEaseType::Linear:
                return FMath::Lerp(Start, End, Alpha);
            case EEaseType::InSine:
                return EaseInSine(Start, End, Alpha);
            case EEaseType::OutSine:
                return EaseOutSine(Start, End, Alpha);
            case EEaseType::InOutSine:
                return EaseInOutSine(Start, End, Alpha);
            case EEaseType::InQuad:
                return EaseInQuad(Start, End, Alpha);
            case EEaseType::OutQuad:
                return EaseOutQuad(Start, End, Alpha);
            case EEaseType::InOutQuad:
                return EaseInOutQuad(Start, End, Alpha);
            case EEaseType::InCubic:
                return EaseInCubic(Start, End, Alpha);
            case EEaseType::OutCubic:
                return EaseOutCubic(Start, End, Alpha);
            case EEaseType::InOutCubic:
                return EaseInOutCubic(Start, End, Alpha);
            case EEaseType::InQuart:
                return EaseInQuart(Start, End, Alpha);
            case EEaseType::OutQuart:
                return EaseOutQuart(Start, End, Alpha);
            case EEaseType::InOutQuart:
                return EaseInOutQuart(Start, End, Alpha);
            case EEaseType::InQuint:
                return EaseInQuint(Start, End, Alpha);
            case EEaseType::OutQuint:
                return EaseOutQuint(Start, End, Alpha);
            case EEaseType::InOutQuint:
                return EaseInOutQuint(Start, End, Alpha);
            case EEaseType::InExpo:
                return EaseInExpo(Start, End, Alpha);
            case EEaseType::OutExpo:
                return EaseOutExpo(Start, End, Alpha);
            case EEaseType::InOutExpo:
                return EaseInOutExpo(Start, End, Alpha);
            default: // Circ
                return FMath::Lerp(Start, End,
                                   1.f - FMath::Sqrt(1.f - FMath::Pow(Alpha * 2.f - 1.f, 2.f)));
        }
    }
    // ---------------------
    // Sine
    // ---------------------

    /**
     * \brief Sine ease-in interpolation.
     */
    static T EaseInSine(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - FMath::Cos(Alpha * PI / 2.f));
    }

    /**
     * \brief Sine ease-out interpolation.
     */
    static T EaseOutSine(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, FMath::Sin(Alpha * PI / 2.f));
    }

    /**
     * \brief Sine ease-in-out interpolation.
     */
    static T EaseInOutSine(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 0.5f * (1.f - FMath::Cos(Alpha * PI)));
    }

    // ---------------------
    // Quadratic
    // ---------------------

    /**
     * \brief Quadratic ease-in interpolation.
     */
    static T EaseInQuad(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, Alpha * Alpha);
    }

    /**
     * \brief Quadratic ease-out interpolation.
     */
    static T EaseOutQuad(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, Alpha * (2.f - Alpha));
    }

    /**
     * \brief Quadratic ease-in-out interpolation.
     */
    static T EaseInOutQuad(T Start, T End, float Alpha)
    {
        float Result = (Alpha < 0.5f)
            ? 2.f * Alpha * Alpha
            : -1.f + (4.f - 2.f * Alpha) * Alpha;
        return FMath::Lerp(Start, End, Result);
    }

    // ---------------------
    // Cubic
    // ---------------------

    /**
     * \brief Cubic ease-in interpolation.
     */
    static T EaseInCubic(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, Alpha * Alpha * Alpha);
    }

    /**
     * \brief Cubic ease-out interpolation.
     */
    static T EaseOutCubic(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - FMath::Pow(1.f - Alpha, 3));
    }

    /**
     * \brief Cubic ease-in-out interpolation.
     */
    static T EaseInOutCubic(T Start, T End, float Alpha)
    {
        float Result = (Alpha < 0.5f)
            ? 4.f * Alpha * Alpha * Alpha
            : 1.f - FMath::Pow(-2.f * Alpha + 2.f, 3.f) / 2.f;
        return FMath::Lerp(Start, End, Result);
    }

    // ---------------------
    // Quartic
    // ---------------------

    /**
     * \brief Quartic ease-in interpolation.
     */
    static T EaseInQuart(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, FMath::Pow(Alpha, 4.f));
    }

    /**
     * \brief Quartic ease-out interpolation.
     */
    static T EaseOutQuart(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - FMath::Pow(1.f - Alpha, 4.f));
    }

    /**
     * \brief Quartic ease-in-out interpolation.
     */
    static T EaseInOutQuart(T Start, T End, float Alpha)
    {
        float Result = (Alpha < 0.5f)
            ? 8.f * FMath::Pow(Alpha, 4.f)
            : 1.f - FMath::Pow(-2.f * Alpha + 2.f, 4.f) / 2.f;
        return FMath::Lerp(Start, End, Result);
    }

    // ---------------------
    // Quintic
    // ---------------------

    /**
     * \brief Quintic ease-in interpolation.
     */
    static T EaseInQuint(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, FMath::Pow(Alpha, 5.f));
    }

    /**
     * \brief Quintic ease-out interpolation.
     */
    static T EaseOutQuint(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - FMath::Pow(1.f - Alpha, 5.f));
    }

    /**
     * \brief Quintic ease-in-out interpolation.
     */
    static T EaseInOutQuint(T Start, T End, float Alpha)
    {
        float Result = (Alpha < 0.5f)
            ? 16.f * FMath::Pow(Alpha, 5.f)
            : 1.f - FMath::Pow(-2.f * Alpha + 2.f, 5.f) / 2.f;
        return FMath::Lerp(Start, End, Result);
    }

    // ---------------------
    // Exponential
    // ---------------------

    /**
     * \brief Exponential ease-in interpolation.
     */
    static T EaseInExpo(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, (Alpha == 0.f) ? 0.f : FMath::Pow(2.f, 10.f * Alpha - 10.f));
    }

    /**
     * \brief Exponential ease-out interpolation.
     */
    static T EaseOutExpo(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, (Alpha == 1.f) ? 1.f : 1.f - FMath::Pow(2.f, -10.f * Alpha));
    }

    /**
     * \brief Exponential ease-in-out interpolation.
     */
    static T EaseInOutExpo(T Start, T End, float Alpha)
    {
        if (Alpha == 0.f) return Start;
        if (Alpha == 1.f) return End;

        float Result = (Alpha < 0.5f)
            ? FMath::Pow(2.f, 20.f * Alpha - 10.f) / 2.f
            : (2.f - FMath::Pow(2.f, -20.f * Alpha + 10.f)) / 2.f;

        return FMath::Lerp(Start, End, Result);
    }

    // ---------------------
    // Circular
    // ---------------------

    /**
     * \brief Circular ease-in interpolation.
     */
    static T EaseInCirc(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - FMath::Sqrt(1.f - Alpha * Alpha));
    }

    /**
     * \brief Circular ease-out interpolation.
     */
    static T EaseOutCirc(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, FMath::Sqrt(1.f - FMath::Pow(Alpha - 1.f, 2.f)));
    }

    /**
     * \brief Circular ease-in-out interpolation.
     */
    static T EaseInOutCirc(T Start, T End, float Alpha)
    {
        float Result = (Alpha < 0.5f)
            ? (1.f - FMath::Sqrt(1.f - 4.f * Alpha * Alpha)) / 2.f
            : (FMath::Sqrt(1.f - FMath::Pow(-2.f * Alpha + 2.f, 2.f)) + 1.f) / 2.f;

        return FMath::Lerp(Start, End, Result);
    }

    // ---------------------
    // Back
    // ---------------------

    /**
     * \brief Back ease-in interpolation.
     */
    static T EaseInBack(T Start, T End, float Alpha)
    {
        constexpr float C1 = 1.70158f;
        constexpr float C3 = C1 + 1.f;
        return FMath::Lerp(Start, End, Alpha * Alpha * (C3 * Alpha - C1));
    }

    /**
     * \brief Back ease-out interpolation.
     */
    static T EaseOutBack(T Start, T End, float Alpha)
    {
        constexpr float C1 = 1.70158f;
        constexpr float C3 = C1 + 1.f;
        float Inv = 1.f - Alpha;
        return FMath::Lerp(Start, End, 1.f - Inv * Inv * (C3 * Inv - C1));
    }

    /**
     * \brief Back ease-in-out interpolation.
     */
    static T EaseInOutBack(T Start, T End, float Alpha)
    {
        constexpr float C1 = 1.70158f;
        constexpr float C2 = C1 * 1.525f;

        float Result = (Alpha < 0.5f)
            ? (FMath::Pow(2.f * Alpha, 2.f) * ((C2 + 1.f) * 2.f * Alpha - C2)) / 2.f
            : (FMath::Pow(2.f * Alpha - 2.f, 2.f) * ((C2 + 1.f) * (Alpha * 2.f - 2.f) + C2) + 2.f) / 2.f;

        return FMath::Lerp(Start, End, Result);
    }

    // ---------------------
    // Elastic
    // ---------------------

    /**
     * \brief Elastic ease-in interpolation.
     */
    static T EaseInElastic(T Start, T End, float Alpha)
    {
        if (Alpha == 0.f) return Start;
        if (Alpha == 1.f) return End;

        constexpr float C4 = (2.f * PI) / 3.f;
        float Result = -FMath::Pow(2.f, 10.f * Alpha - 10.f) * FMath::Sin((Alpha * 10.f - 10.75f) * C4);

        return FMath::Lerp(Start, End, Result);
    }

    /**
     * \brief Elastic ease-out interpolation.
     */
    static T EaseOutElastic(T Start, T End, float Alpha)
    {
        if (Alpha == 0.f) return Start;
        if (Alpha == 1.f) return End;

        constexpr float C4 = (2.f * PI) / 3.f;
        float Result = FMath::Pow(2.f, -10.f * Alpha) * FMath::Sin((Alpha * 10.f - 0.75f) * C4) + 1.f;

        return FMath::Lerp(Start, End, Result);
    }

    /**
     * \brief Elastic ease-in-out interpolation.
     */
    static T EaseInOutElastic(T Start, T End, float Alpha)
    {
        if (Alpha == 0.f) return Start;
        if (Alpha == 1.f) return End;

        constexpr float C5 = (2.f * PI) / 4.5f;

        float Result = (Alpha < 0.5f)
            ? -(FMath::Pow(2.f, 20.f * Alpha - 10.f) * FMath::Sin((20.f * Alpha - 11.125f) * C5)) / 2.f
            : (FMath::Pow(2.f, -20.f * Alpha + 10.f) * FMath::Sin((20.f * Alpha - 11.125f) * C5)) / 2.f + 1.f;

        return FMath::Lerp(Start, End, Result);
    }

    // ---------------------
    // Bounce
    // ---------------------

    /**
     * \brief Bounce ease-out interpolation.
     */
    static T EaseOutBounce(T Start, T End, float Alpha)
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

        return FMath::Lerp(Start, End, Result);
    }

    /**
     * \brief Bounce ease-in interpolation.
     */
    static T EaseInBounce(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - EaseOutBounce(0.f, 1.f, 1.f - Alpha));
    }

    /**
     * \brief Bounce ease-in-out interpolation.
     */
    static T EaseInOutBounce(T Start, T End, float Alpha)
    {
        float Result = (Alpha < 0.5f)
            ? (1.f - EaseOutBounce(0.f, 1.f, 1.f - 2.f * Alpha)) / 2.f
            : (1.f + EaseOutBounce(0.f, 1.f, 2.f * Alpha - 1.f)) / 2.f;

        return FMath::Lerp(Start, End, Result);
    }
};
