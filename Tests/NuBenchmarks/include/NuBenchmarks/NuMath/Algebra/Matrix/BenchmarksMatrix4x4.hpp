// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <benchmark/benchmark.h>
#include <NuBenchmarks/Utils/BenchmarksCommon.hpp>
#include <NuMath/Algebra/Matrix/Matrix4x4.hpp>

namespace NuEngine::Benchmarks
{
    template <>
    inline NuMath::Matrix4x4 MakeTestValue<NuMath::Matrix4x4>(FastRNG& rng)
    {
        (void)rng;

        return NuMath::Matrix4x4(
            2.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 3.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 4.0f, 0.0f,
            10.0f, 20.0f, 30.0f, 1.0f
        );
    }

    void RegisterMatrix4x4Benchmarks();
}