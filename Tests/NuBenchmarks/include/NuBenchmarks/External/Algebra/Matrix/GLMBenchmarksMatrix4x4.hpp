// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#define GLM_FORCE_INTRINSICS 
#define GLM_FORCE_SSE42 
#define GLM_FORCE_INLINE 
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 

#include <glm/glm.hpp>
#include <benchmark/benchmark.h>
#include <NuBenchmarks/Utils/BenchmarksCommon.hpp>

namespace NuEngine::Benchmarks
{
    // ВИПРАВЛЕНО: додано аргумент (FastRNG& rng)
    template <>
    inline glm::mat4 MakeTestValue<glm::mat4>(FastRNG& rng)
    {
        (void)rng; // Приглушуємо warning про невикористану змінну

        return glm::mat4(
            2.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 3.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 4.0f, 0.0f,
            10.0f, 20.0f, 30.0f, 1.0f
        );
    }

    void RegisterMatrix4x4Benchmarks_GLM();
}