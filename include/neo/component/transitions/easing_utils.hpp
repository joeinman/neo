/*
 * Copyright (c) 2025, Joe Inman
 *
 * Licensed under the MIT License.
 * You may obtain a copy of the License at:
 *     https://opensource.org/licenses/MIT
 *
 * This file is part of the Neo Library.
 */

#pragma once

#include <cmath>

namespace jsi::neo::easing
{

// Quadratic easing functions
inline double easeInQuad(double t)
{
    return t * t;
}

inline double easeOutQuad(double t)
{
    return t * (2.0 - t);
}

inline double easeInOutQuad(double t)
{
    if (t < 0.5)
        return 2.0 * t * t;
    return -1.0 + (4.0 - 2.0 * t) * t;
}

// Cubic easing functions
inline double easeInCubic(double t)
{
    return t * t * t;
}

inline double easeOutCubic(double t)
{
    t = t - 1.0;
    return t * t * t + 1.0;
}

inline double easeInOutCubic(double t)
{
    if (t < 0.5)
        return 4.0 * t * t * t;
    t = t - 1.0;
    return 1.0 + t * t * t * 4.0;
}

// Exponential easing functions
inline double easeInExpo(double t)
{
    return (t == 0.0) ? 0.0 : std::pow(2.0, 10.0 * (t - 1.0));
}

inline double easeOutExpo(double t)
{
    return (t == 1.0) ? 1.0 : 1.0 - std::pow(2.0, -10.0 * t);
}

// Additional easing functions that could be useful
inline double easeInOutExpo(double t)
{
    if (t == 0.0 || t == 1.0)
        return t;

    if (t < 0.5)
        return 0.5 * std::pow(2.0, 20.0 * t - 10.0);

    return 0.5 * (2.0 - std::pow(2.0, -20.0 * t + 10.0));
}

}  // namespace jsi::neo::easing
