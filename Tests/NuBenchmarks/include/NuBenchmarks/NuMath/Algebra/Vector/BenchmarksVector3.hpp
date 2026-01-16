// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <benchmark/benchmark.h>
#include <NuBenchmarks/Utils/BenchmarksCommon.hpp>
#include <NuMath/Algebra/Vector/Vector3.hpp>

namespace NuEngine::Benchmarks
{
    template <>
    inline NuMath::Vector3 MakeTestValue<NuMath::Vector3>(FastRNG& rng)
    {
        (void)rng;
        return NuMath::Vector3(1.0f, 2.0f, 3.0f);
    }

    void RegisterVector3Benchmarks();
}