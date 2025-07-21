#pragma once

#include "CoreMinimal.h"

template <typename T, typename = typename TEnableIf<TIsArithmetic<T>::Value>::Type>
class QUICKTWEEN_API FEaseFunctions
{
public:

    // ---------------------
    // Sine
    // ---------------------
    static T EaseInSine(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - FMath::Cos(Alpha * PI / 2.f));
    }

    static T EaseOutSine(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, FMath::Sin(Alpha * PI / 2.f));
    }

    static T EaseInOutSine(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 0.5f * (1.f - FMath::Cos(Alpha * PI)));
    }

    // ---------------------
    // Quadratic
    // ---------------------
    static T EaseInQuad(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, Alpha * Alpha);
    }

    static T EaseOutQuad(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, Alpha * (2.f - Alpha));
    }

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
    static T EaseInCubic(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, Alpha * Alpha * Alpha);
    }

    static T EaseOutCubic(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - FMath::Pow(1.f - Alpha, 3));
    }

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
    static T EaseInQuart(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, FMath::Pow(Alpha, 4.f));
    }

    static T EaseOutQuart(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - FMath::Pow(1.f - Alpha, 4.f));
    }

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
    static T EaseInQuint(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, FMath::Pow(Alpha, 5.f));
    }

    static T EaseOutQuint(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - FMath::Pow(1.f - Alpha, 5.f));
    }

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
    static T EaseInExpo(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, (Alpha == 0.f) ? 0.f : FMath::Pow(2.f, 10.f * Alpha - 10.f));
    }

    static T EaseOutExpo(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, (Alpha == 1.f) ? 1.f : 1.f - FMath::Pow(2.f, -10.f * Alpha));
    }

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
    static T EaseInCirc(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - FMath::Sqrt(1.f - Alpha * Alpha));
    }

    static T EaseOutCirc(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, FMath::Sqrt(1.f - FMath::Pow(Alpha - 1.f, 2.f)));
    }

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
    static T EaseInBack(T Start, T End, float Alpha)
    {
        constexpr float C1 = 1.70158f;
        constexpr float C3 = C1 + 1.f;
        return FMath::Lerp(Start, End, Alpha * Alpha * (C3 * Alpha - C1));
    }

    static T EaseOutBack(T Start, T End, float Alpha)
    {
        constexpr float C1 = 1.70158f;
        constexpr float C3 = C1 + 1.f;
        float Inv = 1.f - Alpha;
        return FMath::Lerp(Start, End, 1.f - Inv * Inv * (C3 * Inv - C1));
    }

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
    static T EaseInElastic(T Start, T End, float Alpha)
    {
        if (Alpha == 0.f) return Start;
        if (Alpha == 1.f) return End;

        constexpr float C4 = (2.f * PI) / 3.f;
        float Result = -FMath::Pow(2.f, 10.f * Alpha - 10.f) * FMath::Sin((Alpha * 10.f - 10.75f) * C4);

        return FMath::Lerp(Start, End, Result);
    }

    static T EaseOutElastic(T Start, T End, float Alpha)
    {
        if (Alpha == 0.f) return Start;
        if (Alpha == 1.f) return End;

        constexpr float C4 = (2.f * PI) / 3.f;
        float Result = FMath::Pow(2.f, -10.f * Alpha) * FMath::Sin((Alpha * 10.f - 0.75f) * C4) + 1.f;

        return FMath::Lerp(Start, End, Result);
    }

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

    static T EaseInBounce(T Start, T End, float Alpha)
    {
        return FMath::Lerp(Start, End, 1.f - EaseOutBounce(0.f, 1.f, 1.f - Alpha));
    }

    static T EaseInOutBounce(T Start, T End, float Alpha)
    {
        float Result = (Alpha < 0.5f)
            ? (1.f - EaseOutBounce(0.f, 1.f, 1.f - 2.f * Alpha)) / 2.f
            : (1.f + EaseOutBounce(0.f, 1.f, 2.f * Alpha - 1.f)) / 2.f;

        return FMath::Lerp(Start, End, Result);
    }
};
