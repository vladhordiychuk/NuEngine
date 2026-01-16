// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <DirectXMath.h>
#include <benchmark/benchmark.h>
#include <NuBenchmarks/Utils/BenchmarksCommon.hpp>

namespace NuEngine::Benchmarks
{
    // ВИПРАВЛЕНО: додано аргумент (FastRNG& rng)
    template<>
    inline DirectX::XMFLOAT4 MakeTestValue<DirectX::XMFLOAT4>(FastRNG& rng)
    {
        // ВИПРАВЛЕНО: Використовуємо rng замість повільного rand(), 
        // щоб умови тестування були однаковими для всіх бібліотек.
        return DirectX::XMFLOAT4(
            rng.NextFloat(),
            rng.NextFloat(),
            rng.NextFloat(),
            rng.NextFloat()
        );
    }

    void RegisterVector4Benchmarks_DirectX();
}