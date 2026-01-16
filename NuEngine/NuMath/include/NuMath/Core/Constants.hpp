// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <limits>

#ifdef min
    #undef min
#endif
#ifdef max
    #undef max
#endif

namespace NuMath
{
    constexpr float PI      = 3.14159265358979323846f;
    constexpr float TWO_PI  = 6.28318530717958647692f;
    constexpr float HALF_PI = 1.57079632679489661923f;
    constexpr float INV_PI  = 0.31830988618379067154f;

    constexpr float DEG_TO_RAD = 0.01745329251994329577f;
    constexpr float RAD_TO_DEG = 57.2957795130823208768f;

    constexpr float EPSILON = 1.192092896e-07F;

    constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
    constexpr float FLOAT_MIN = std::numeric_limits<float>::lowest();

} // namespace NuMath