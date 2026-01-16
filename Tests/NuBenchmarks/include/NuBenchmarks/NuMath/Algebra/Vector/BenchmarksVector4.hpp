// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <benchmark/benchmark.h>
#include <NuBenchmarks/Utils/BenchmarksCommon.hpp>
#include <NuMath/Algebra/Vector/Vector4.hpp>
#include <NuMath/Batch/Vector/VectorBatch.hpp>
#include <NuMath/Batch/Vector/VectorBatchSoA.hpp>

namespace NuEngine::Benchmarks
{
    template <>
    inline NuMath::Vector4 MakeTestValue<NuMath::Vector4>(FastRNG& rng)
    {
        (void)rng;
        return NuMath::Vector4(1.0f, 2.0f, 3.0f, 4.0f);
    }

    void RegisterVector4Benchmarks();
}