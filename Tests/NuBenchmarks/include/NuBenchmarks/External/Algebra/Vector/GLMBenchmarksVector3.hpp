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
    template<>
    inline glm::vec3 MakeTestValue<glm::vec3>(FastRNG& rng)
    {
        (void)rng;
        return glm::vec3(1.0f, 2.0f, 3.0f);
    }

    void RegisterVector3Benchmarks_GLM();
}