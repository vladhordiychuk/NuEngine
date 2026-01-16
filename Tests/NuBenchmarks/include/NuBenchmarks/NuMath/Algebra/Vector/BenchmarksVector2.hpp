// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <benchmark/benchmark.h>
#include <NuBenchmarks/Utils/BenchmarksCommon.hpp>
#include <NuMath/Algebra/Vector/Vector2.hpp>

namespace NuEngine::Benchmarks
{
    template <>
    inline NuMath::Vector2 MakeTestValue<NuMath::Vector2>(FastRNG& rng)
    {
        (void)rng;
        return NuMath::Vector2(1.0f, 2.0f);
    }

    void RegisterVector2Benchmarks();
}